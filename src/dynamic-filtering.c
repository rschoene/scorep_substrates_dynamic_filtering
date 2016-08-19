#define UNW_LOCAL_ONLY
#define _GNU_SOURCE /* <- needed for libunwind so stack_t is known */

#include <stddef.h> /* <- needs to come before libunwind for size_t */
#include <libunwind.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include <scorep_substrates_definition.h>

/**
 * Stores region info.
 */
typedef struct region_info
{
    /** Global counter for region entries */
    uint64_t call_cnt;
    /** Global calculated region duration */
    uint64_t duration;
    /** Linked list next pointer */
    struct region_info* nxt;
    /** Pointer to the callq for the enter instrumentation function */
    char* enter_func;
    /** Pointer to the callq for the exit instrumentation function */
    char* exit_func;
    /** Human readable name of the region */
    const char* region_name;
    /** Handle identifying the region */
    uint32_t region_handle;
    /** Recursion depth in this region */
    uint32_t depth;
    /** Mean region duration used for comparison */
    float mean_duration;
    /** Marks whether the region is deletable */
    bool deletable;
    /** Marks whether the region has been deleted */
    bool inactive;
} region_info;

/**
 * Stores calling information per thread.
 *
 * This is necessary as calculating the duration for one function call would be impossible otherwise
 * in a multithreaded environment.
 */
typedef struct local_region_info
{
    /** Timestamp of last enter into this region */
    uint64_t last_enter;
    /** Linked list next pointer */
    struct local_region_info* nxt;
    /** Region id this info belongs to */
    uint32_t region_handle;
} local_region_info;

/** Global list of defined regions */
region_info* regions = NULL;

/** Thread local region info */
__thread local_region_info* local_info = NULL;

/** General mutex used as a guard for writing region info */
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/** Special mutex used for stopping new threads entering regions while deleting calls */
pthread_mutex_t deletion_barrier = PTHREAD_MUTEX_INITIALIZER;

/** Thread counter */
uint64_t thread_ctr = 0;

/** Flag indicating we're deletion ready */
bool deletion_ready = false;

/** Flag indicating the filtering method to be used (true = absolute, false = relative) */
bool filtering_absolute;

/** Threshold for filtering */
unsigned long long threshold = 0;

/** Mean duration across all regions */
float mean_duration = 0;

/**
 * Update the mean duration of all regions.
 *
 * Only used if the plugin is compiled with FILTERING_RELATIVE.
 */
static void update_mean_duration( )
{
    region_info* current = regions;
    uint64_t ctr = 1;
    float new_duration = current->mean_duration;

    while( current->nxt != NULL )
    {
        current = current->nxt;
        ctr++;
        new_duration += current->mean_duration;
    }

    mean_duration = new_duration / ctr;
}

/**
 * Overrides a callq at the given position with a five byte NOP.
 *
 * By calling mprotect right before and after writing the NOP this function ensures that the correct
 * part of the TXT segment is writable and it's only writable as long as needed.
 *
 * @param   ptr                             Position of the callq to override.
 */
static void override_callq( char*                                                   ptr )
{
    // Get the page size of the system we're running on.
    int page_size = sysconf( _SC_PAGE_SIZE );
    // The callq may be splitted onto two different pages and mprotect changes access permissions
    // on a per page basis, so we need to change the access permission on the pages where the first
    // and the last byte of the callq reside.
    void* first_ptr = ptr - ( (unsigned long) ptr % page_size );
    void* second_ptr = ( ptr + 4 ) - ( (unsigned long) ( ptr + 4 ) % page_size );

    // Add the write permission.
    if( mprotect( first_ptr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC ) != 0
     || mprotect( second_ptr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC ) != 0 )
    {
        fprintf( stderr,  "Could not add write permission to memory access rights on position "
                          "%p", ptr );
    }

    // Finally write that NOP.
    memset( ptr,     0x0f, 1 );
    memset( ptr + 1, 0x1f, 1 );
    memset( ptr + 2, 0x44, 1 );
    memset( ptr + 3, 0x00, 1 );
    memset( ptr + 4, 0x00, 1 );

    // Remove the write permission.
    if( mprotect( first_ptr, page_size, PROT_READ | PROT_EXEC ) != 0
     || mprotect( second_ptr, page_size, PROT_READ | PROT_EXEC ) != 0 )
    {
        fprintf( stderr,  "Could not remove write permission to memory access rights on position "
                          "%p", ptr );
    }
}

/**
 * Returns the instruction pointer for the given function.
 *
 * This method walks down the call path and tries to find a function with the given name and
 * returns a pointer for the first byte of the call to this function in the current call path.
 * Note that only the first (beginning from the innermost function) occurrence of the function call
 * will be handled.
 *
 * @param   function_name                   The function to look up.
 *
 * @return                                  Pointer to the first byte of the call to the given
 *                                          function in the current call path.
 */
static char* get_function_call_ip( const char*                                      function_name )
{
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip, offset;
    char sym[256];

    unw_getcontext( &uc );
    unw_init_local( &cursor, &uc );

    // Step up the call path...
    do
    {
        // ...and check if the current entry has the name given in the parameter.
        if( unw_get_proc_name( &cursor, sym, sizeof( sym ), &offset ) == 0
            && strcmp( sym, function_name ) == 0 )
        {
            unw_get_reg( &cursor, UNW_REG_IP, &ip );

            // UNW_REG_IP is the first byte _after_ the callq so we need to step back 5 bytes.
            return (char*) ( ip - 5 );
        }
    } while( unw_step( &cursor ) > 0 );

    // This shouldn't happen, if we're on this point, we tried to delete a function not present in
    // the current call path. We return zero in this case because delete_regions wont try to delete
    // this call in this case and we get no undefined behaviour.
    fprintf( stderr, "Function name not found in call path. This shouldn't happen.\n" );
    return (char*) 0;
}

/**
 * Remove all unwanted regions.
 *
 * This function iterates over all regions and deletes all of them that are marked as deletable.
 */
static void delete_regions( )
{
    pthread_mutex_lock( &mtx );

    region_info* current = regions;

    while( current != NULL )
    {
        // Only delete the function calls if the region is marked as deletable, the address of the
        // entry function call is correctly set and the address of the exit function call is
        // correctly set.
        if( current->deletable && !( current->enter_func == 0 || current->exit_func == 0 ) && current->depth == 1 )
        {
            override_callq( current->enter_func );
            override_callq( current->exit_func );
            current->inactive = true;
            fprintf( stderr, "Deleted instrumentation calls for region %s!\n",
                                                                        current->region_name );
        }

        current = current->nxt;
    }

    pthread_mutex_unlock( &mtx );
}

/**
 * Get internal pointer to region info for given region handle.
 *
 * @param   region_handle                   The region to look up.
 *
 * @return                                  The corresponding region info.
 */
static inline region_info* get_region_info( uint32_t                                region_handle )
{
    region_info* current_region = regions;

    // Select the correct region
    while( current_region->region_handle != region_handle )
    {
        current_region = current_region->nxt;
    }

    return current_region;
}

/**
 * Get the thread local information for the given region.
 *
 * This is the correct way to access the thread local information for a region. This function checks
 * if the calling thread already has information about the region asked for and creates it if
 * needed.
 *
 * @param   region_handle                   The region to access the thread local information for.
 *
 * @return                                  The thread local information for the given region and
 *                                          the calling thread.
 */
static local_region_info* get_local_info( uint32_t                                  region_handle )
{
    local_region_info* current = local_info;

    // Find the correct local region info
    while( current != NULL )
    {
        if( current->region_handle == region_handle )
        {
            break;
        }
        else
        {
            current = current->nxt;
        }
    }

    if( current != NULL )
    {
        // We've found it? Return it!
        return current;
    }
    else
    {
        // We haven't found it? Create a new one and append it correctly to our thread local list.
        if( local_info == NULL )
        {
            // Seems to be the first element for this thread so use it as the start of the list.
            local_info = calloc( 1, sizeof( local_region_info ) );
            local_info->region_handle = region_handle;
            return local_info;
        }
        else
        {
            // There already are some elements so append the new one at the end of the list.
            current = local_info;
            local_region_info* tmp = calloc( 1, sizeof( local_region_info ) );
            tmp->region_handle = region_handle;

            while( current->nxt != NULL )
            {
                current = current->nxt;
            }

            current->nxt = tmp;
            return tmp;
        }
    }
}

/**
 * Thread fork event.
 *
 * As long as there's more than one thread, there's no save way to make any changes to their common
 * TXT segment. So we have to notice the creation of threads and wait untill all of them have
 * joined.
 *
 * @param   scorep_location                 unused
 * @param   timestamp                       unused
 * @param   scorep_paradigm                 unused
 * @param   scorep_thread_team              unused
 */
static void on_team_begin( __attribute__((unused)) struct SCOREP_Location*          scorep_location,
                           __attribute__((unused)) uint64_t                         timestamp,
                           __attribute__((unused)) SCOREP_ParadigmType              scorep_paradigm,
                           __attribute__((unused)) SCOREP_InterimCommunicatorHandle scorep_thread_team )
{
    pthread_mutex_lock( &deletion_barrier );
    thread_ctr++;
    pthread_mutex_unlock( &deletion_barrier );
}

/**
 * Thread join event.
 *
 * See on_team_begin.
 *
 * @param   scorep_location                 unused
 * @param   timestamp                       unused
 * @param   scorep_paradigm                 unused
 * @param   scorep_thread_team              unused
 */
static void on_team_end( __attribute__((unused)) struct SCOREP_Location*            scorep_location,
                         __attribute__((unused)) uint64_t                           timestamp,
                         __attribute__((unused)) SCOREP_ParadigmType                scorep_paradigm,
                         __attribute__((unused)) SCOREP_InterimCommunicatorHandle   scorep_thread_team )
{
    pthread_mutex_lock( &deletion_barrier );
    thread_ctr--;
    pthread_mutex_unlock( &deletion_barrier );

    // Free the thread local storage
    local_region_info* current = local_info;
    local_region_info* tmp;

    while( current != NULL )
    {
        tmp = current;
        current = current->nxt;
        free( tmp );
    }

    local_info = NULL;
}

/**
 * Enter region.
 *
 * This updates the region info (call counter) and decides whether or not the region instrumentation
 * should be overwritten. If so, it checks if we're in a multi threaded environment and acts
 * accordingly:
 *
 *  * multithreaded: Skip deletion and wait untill all threads are joined.
 *  * singlethreaded: Mark the state as deletion ready and hold a mutex so that no new spawned
 *    thread can enter a possibly deleted region.
 *
 * @param   scorep_location                 unused
 * @param   timestamp                       The timestamp of the entry event.
 * @param   region_handle                   The region that is entered.
 * @param   metric_values                   unused
 */
static void on_enter_region( __attribute__((unused)) struct SCOREP_Location*        scorep_location,
                             uint64_t                                               timestamp,
                             SCOREP_RegionHandle                                    region_handle,
                             __attribute__((unused)) uint64_t*                      metric_values )
{
    region_info* region = get_region_info( region_handle );

    // If the current region is already deleted, skip this whole thing.
    if( !region->inactive )
    {
        local_region_info* info = get_local_info( region_handle );

        // Store the last (this) entry for the current thread.
        info->last_enter = timestamp;

        //fprintf( stderr, "Waiting for mtx in enter (%d)\n", region_handle );
        pthread_mutex_lock( &mtx );

        region->depth++;

        // This region is marked for deletion but not already deleted.
        if( region->deletable && !region->enter_func )
        {
            region->enter_func = get_function_call_ip( region->region_name );
        }

        pthread_mutex_unlock( &mtx );

        if( thread_ctr == 0 && !deletion_ready )
        {
            fprintf( stderr, "Waiting for deletion barrier.\n" );
            // Lock all new threads from entering any region.
            pthread_mutex_lock( &deletion_barrier );

            if( thread_ctr == 0 )
            {
                // We're the only thread, so keep the mutex and mark that we're deletion ready.
                deletion_ready = true;
            }
            else
            {
                // There are other threads around so deletion is impossible. Unlock the mutex and carry
                // on.
                pthread_mutex_unlock( &deletion_barrier );
            }
        }
    }
}

/**
 * Exit region.
 *
 * This method contains the code for calculating metrics (see on_enter as well) and the actual
 * instrumentation override code.
 *
 * @param   scorep_location                 unused
 * @param   timestamp                       Time of the exit from the region.
 * @param   region_handle                   The region that is exited.
 * @param   metric_values                   unused
 */
static void on_exit_region( __attribute__((unused)) struct SCOREP_Location*         scorep_location,
                            uint64_t                                                timestamp,
                            SCOREP_RegionHandle                                     region_handle,
                            __attribute__((unused)) uint64_t*                       metric_values )
{
    if( deletion_ready )
    {
        // The current state is ready for deletion, so delete all regions ready for deletion.
        delete_regions( );
        // Reset the deletion ready flag
        deletion_ready = false;
        // Reset mutex so normal program flow can be reached again.
        pthread_mutex_unlock( &deletion_barrier );
    }

    region_info* region = get_region_info( region_handle );
    region->depth--;

    // If the region already has been deleted or marked as deletable, skip the next steps.
    if( !region->inactive && !region->deletable )
    {
        local_region_info* info = get_local_info( region_handle );

        //fprintf( stderr, "Waiting for mtx in exit (%d).\n", region_handle );
        pthread_mutex_lock( &mtx );

        // Region not (yet) ready for deletion so update the metrics.
        region->call_cnt++;
        region->duration += ( timestamp - info->last_enter );

        if( filtering_absolute )
        {
            // We're filtering absolute so just compare this region's mean duration with the
            // threshold.
            if( region->duration / region->call_cnt < threshold )
            {
                region->exit_func = get_function_call_ip( region->region_name );
                region->deletable = true;
            }
        }
        else
        {
            // We're filtering relative so first update all regions' mean durations and then compare
            // the duration of this region with the mean of all regions.
            region->mean_duration = region->duration / region->call_cnt;

            update_mean_duration( );

            if( region->mean_duration < mean_duration - threshold )
            {
                region->exit_func = get_function_call_ip( region->region_name );
                region->deletable = true;
            }
        }

        pthread_mutex_unlock( &mtx );
    }
}

/**
 * Call on Score-P's region definition event.
 *
 * Creates a new region_info struct in the globel regions list for the newly defined region.
 */
static void on_define_region( __attribute__((unused)) const char*                   region_name,
                              __attribute__((unused)) const char*                   region_canonical_name,
                              __attribute__((unused)) SCOREP_ParadigmType           paradigm_type,
                              __attribute__((unused)) SCOREP_RegionType             region_type,
                              SCOREP_RegionHandle                                   region_handle )
{
    pthread_mutex_lock( &mtx );

    region_info* last = NULL;
    region_info* current = regions;

    while( current != NULL )
    {
        last = current;
        current = current->nxt;
    }

    if( last == NULL )
    {
        regions = calloc( 1, sizeof( region_info ) );
        regions->region_handle = region_handle;
        regions->region_name = region_name;
    }
    else
    {
        last->nxt = calloc( 1, sizeof( region_info ) );
        current = last->nxt;
        current->region_handle = region_handle;
        current->region_name = region_name;
    }

    pthread_mutex_unlock( &mtx );
}

/**
 * The plugin's initialization method.
 *
 * Just sets some default values and reads some environment variables.
 */
static void on_init( )
{
    // Get the threshold for filtering
    char* env_str = getenv( "SCOREP_SUBSTRATES_DYNAMIC_FILTERING_THRESHOLD" );
    if( env_str == NULL )
    {
        fprintf( stderr, "Unable to parse SCOREP_SUBSTRATES_DYNAMIC_FILTERING_THRESHOLD.\n" );
        exit( EXIT_FAILURE );
    }

    threshold = strtoull( env_str, NULL, 10 );
    if( threshold == 0 )
    {
        fprintf( stderr, "Unable to parse SCOREP_SUBSTRATES_DYNAMIC_FILTERING_THRESHOLD or set "
                         "to 0.\n" );
        exit( EXIT_FAILURE );
    }

    // Get the wanted filtering method
    env_str = getenv( "SCOREP_SUBSTRATES_DYNAMIC_FILTERING_METHOD" );
    if( env_str == NULL )
    {
        fprintf( stderr, "Unable to parse SCOREP_SUBSTRATES_DYNAMIC_FILTERING_METHOD.\n" );
        exit( EXIT_FAILURE );
    }

    if( strcmp( env_str, "absolute" ) == 0 )
    {
        filtering_absolute = true;
    }
    else
    {
        filtering_absolute = false;
    }
}

/**
 * Finalizing method.
 *
 * Mainly used for cleanup.
 */
static void on_finalize( )
{
    region_info* current = regions;
    region_info* tmp;

    while( current != NULL )
    {
        tmp = current;
        current = current->nxt;
        free( tmp );
    }

    regions = NULL;
}

SCOREP_Substrates_Callback** SCOREP_SubstratePlugin_dynamic_filtering_plugin_get_event_callbacks( )
{
    int i;
    SCOREP_Substrates_Callback** retval = malloc( SCOREP_SUBSTRATES_NUM_MODES *
                                                  sizeof( SCOREP_Substrates_Callback* ) );

    for( i = 0; i < SCOREP_SUBSTRATES_NUM_MODES; i++ )
    {
        retval[i] = calloc( SCOREP_SUBSTRATES_NUM_EVENTS, sizeof( SCOREP_Substrates_Callback ) );
    }

    retval[SCOREP_SUBSTRATES_RECORDING_ENABLED][SCOREP_EVENT_ENTER_REGION] =
                                                        (SCOREP_Substrates_Callback) on_enter_region;
    retval[SCOREP_SUBSTRATES_RECORDING_ENABLED][SCOREP_EVENT_EXIT_REGION] =
                                                        (SCOREP_Substrates_Callback) on_exit_region;
    retval[SCOREP_SUBSTRATES_RECORDING_ENABLED][SCOREP_EVENT_THREAD_FORK_JOIN_TEAM_BEGIN] =
                                                        (SCOREP_Substrates_Callback) on_team_begin;
    retval[SCOREP_SUBSTRATES_RECORDING_ENABLED][SCOREP_EVENT_THREAD_FORK_JOIN_TEAM_END] =
                                                        (SCOREP_Substrates_Callback) on_team_end;
    return retval;
}

SCOREP_Substrates_Callback* SCOREP_SubstratePlugin_dynamic_filtering_plugin_get_mgmt_callbacks( )
{
  SCOREP_Substrates_Callback* retval = malloc( SCOREP_SUBSTRATES_NUM_MGMT_EVENTS *
                                               sizeof( SCOREP_Substrates_Callback ) );
  retval[SCOREP_EVENT_FINALIZE_SUBSTRATE] = (SCOREP_Substrates_Callback) on_finalize;
  retval[SCOREP_EVENT_DEFINE_REGION] = (SCOREP_Substrates_Callback) on_define_region;
  return retval;
}

int SCOREP_SubstratePlugin_dynamic_filtering_plugin_init( )
{
  on_init( );
  return 0;
}
