#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __STDC_NO_ATOMICS__
#error "C11 conform compiler with atomics support needed.\n"
#else
#include <stdatomic.h>
#endif

#include <scorep_substrates_definition.h>

#include "delete-call.h"

/**
 * I don't set a default because I'd like to have a non filtering version of this plugin. In case
 * of a later production use it could be clever to set a default measurement method and remove
 * the "define || define" checks in the code for better maintainability.
 */
#if defined( FILTERING_RELATIVE ) && defined( FILTERING_ABSOLUTE )
#error "Can't compile a relative and an absolute filtering plugin at the same time.\n"
#endif

/**
 * Stores region info.
 */
typedef struct region_info
{
    /** Handle identifying the region */
    uint64_t region_handle;
    /** Global counter for region entries */
    uint64_t call_cnt;
    /** Global calculated region duration */
    uint64_t duration;
#ifdef FILTERING_RELATIVE
    /** Mean region duration used for comparison */
    float mean_duration;
#endif
    /** Linked list next pointer */
    struct region_info* nxt;
    /** Pointer to the callq for the enter instrumentation function */
    char* enter_func;
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
    /** Region id this info belongs to */
    uint64_t region_handle;
    /** Timestamp of last enter into this region */
    uint64_t last_enter;
    /** Linked list next pointer */
    struct local_region_info* nxt;
} local_region_info;

/** Global list of defined regions */
region_info* regions = NULL;

/** Thread local region info */
__thread local_region_info* local_info = NULL;

/** General mutex used as a guard for writing region info */
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/** Special mutex used for stopping new threads entering regions while deleting calls */
pthread_mutex_t deletion_barrier = PTHREAD_MUTEX_INITIALIZER;

/** Atomic thread counter */
atomic_uint_fast64_t thread_ctr = ATOMIC_VAR_INIT( 0 );

/** Atomic flag indicating we're deletion ready */
atomic_flag deletion_ready = ATOMIC_FLAG_INIT;

/** Threshold for filtering */
unsigned long long threshold = 0;

#ifdef FILTERING_RELATIVE
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
#endif /* FILTERING_RELATIVE */

/**
 * Get internal pointer to region info for given region handle.
 *
 * @param   region_handle                   The region to look up.
 *
 * @return                                  The corresponding region info.
 */
static inline region_info* get_region_info( uint64_t                                region_handle )
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
static local_region_info* get_local_info( uint64_t                                  region_handle )
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
            local_info = calloc( 1, sizeof( local_region_info ) );
            local_info->region_handle = region_handle;
            return local_info;
        }
        else
        {
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
 * As long as there's more than one thread, there's no save way to make any changes to their common
 * TXT segment. So we have to notice the creation of threads and wait untill all of them have
 * joined.
 */
static void on_team_begin( __attribute__((unused)) struct SCOREP_Location*          scorep_location,
                           __attribute__((unused)) uint64_t                         timestamp,
                           __attribute__((unused)) SCOREP_ParadigmType              scorep_paradigm,
                           __attribute__((unused)) SCOREP_InterimCommunicatorHandle scorep_thread_team )
{
    // Thread initialization is protected by a mutex because otherwise threads could start to run
    // while we want to delete something.
    pthread_mutex_lock( &deletion_barrier );
    atomic_fetch_add( &thread_ctr, 1 );
    pthread_mutex_unlock( &deletion_barrier );
}

/**
 * See above.
 */
static void on_team_end( __attribute__((unused)) struct SCOREP_Location*            scorep_location,
                         __attribute__((unused)) uint64_t                           timestamp,
                         __attribute__((unused)) SCOREP_ParadigmType                scorep_paradigm,
                         __attribute__((unused)) SCOREP_InterimCommunicatorHandle   scorep_thread_team )
{
    // Thread joining doesn't have to be protected, because if there are threads to join, we'll
    // never get into deletion.
    atomic_fetch_sub( &thread_ctr, 1 );
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
 */
static void on_enter( __attribute__((unused)) struct SCOREP_Location*               scorep_location,
                      uint64_t                                                      timestamp,
                      SCOREP_RegionHandle                                           region_handle,
                      __attribute__((unused)) uint64_t*                             metric_values )
{
    region_info* region = get_region_info( region_handle );

    // If the current region is already deleted, skip this whole thing.
    if( !region->inactive )
    {
        local_region_info* info = get_local_info( region_handle );

        pthread_mutex_lock( &mtx );

        // Store the last (this) entry for the current thread.
        info->last_enter = timestamp;

        // This region is marked for deletion but not already deleted.
        if( region->deletable )
        {
            // Lock all new threads from entering any region.
            pthread_mutex_lock( &deletion_barrier );

            uint_fast64_t expection = 0;
            if( atomic_compare_exchange_weak( &thread_ctr, &expection, 0 ) )
            {
                // We're the only thread, so keep the mutex and mark that we're deletion ready.
                region->enter_func = get_function_call_ip( "__cyg_profile_enter_func" );
                atomic_flag_test_and_set( &deletion_ready );
            }
            else
            {
                // There are other threads around so deletion is impossible. Unlock the mutex and
                // carry on.
                pthread_mutex_unlock( &deletion_barrier );
            }
        }

        pthread_mutex_unlock( &mtx );
    }
}

/**
 * Exit region.
 *
 * This method contains the code for calculating metrics (see on_enter as well) and the actual
 * instrumentation override code.
 */
static void on_exit( __attribute__((unused)) struct SCOREP_Location*                scorep_location,
                     uint64_t                                                       timestamp,
                     SCOREP_RegionHandle                                            region_handle,
                     __attribute__((unused)) uint64_t*                              metric_values )
{
    region_info* region = get_region_info( region_handle );

    if( !region->inactive )
    {
        local_region_info* info = get_local_info( region_handle );

        pthread_mutex_lock( &mtx );

        // Region is marked as deletable...
        if( region->deletable )
        {
            // ...and the current state is ready for deletion
            if( atomic_flag_test_and_set( &deletion_ready ) )
            {
                // Set region to inactive so further processing of this region is skipped.
                region->inactive = true;
                // Reset mutex so normal program flow can be reached again.
                pthread_mutex_unlock( &deletion_barrier );
            }
        }
        else
        {
            region->call_cnt++;
            region->duration += ( timestamp - info->last_enter );

#ifdef FILTERING_RELATIVE
            region->mean_duration = region->duration / region->call_cnt;

            update_mean_duration( );

            if( region->mean_duration < mean_duration - threshold )
#endif /* FILTERING_RELATIVE */
#ifdef FILTERING_ABSOLUTE
            if( region->duration / region->call_cnt < threshold )
#endif /* FILTERING_ABSOLUTE */
            {
                region->deletable = true;
            }
        }

        // After atomic_flag_test_and_set deletion_ready is always true whether or not it was before.
        // In neither case we want it to be true after on_exit
        atomic_flag_clear( &deletion_ready );

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
    }
    else
    {
        last->nxt = calloc( 1, sizeof( region_info ) );
        current = last->nxt;
        current->region_handle = region_handle;
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
    // Set deletion ready to the default false state.
    atomic_flag_clear( &deletion_ready );
    // Get the threshold for filtering
    char* env_str = getenv( "SCOREP_SUBSTRATES_DYNAMIC_FILTERING_THRESHOLD" );
    if( env_str == NULL )
    {
        fprintf( stderr, "Unable to parse SCOREP_SUBSTRATES_DYNAMIC_FILTERING_THRESHOLD.\n" );
        exit( EXIT_FAILURE );
    }
    else
    {
        threshold = strtoull( env_str, NULL, 10 );
        if( threshold == 0 )
        {
            fprintf( stderr, "Unable to parse SCOREP_SUBSTRATES_DYNAMIC_FILTERING_THRESHOLD or set "
                             "to 0.\n" );
            exit( EXIT_FAILURE );
        }
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
                                                        (SCOREP_Substrates_Callback) on_enter;
    retval[SCOREP_SUBSTRATES_RECORDING_ENABLED][SCOREP_EVENT_EXIT_REGION] =
                                                        (SCOREP_Substrates_Callback) on_exit;
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
