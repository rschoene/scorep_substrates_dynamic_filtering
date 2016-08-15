#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __STDC_NO_ATOMICS__
#error "C11 conform compiler with atomics support needed.\n"
#else
#include <stdatomic.h>
#endif

#include <SCOREP_Location.h>
#include <SCOREP_Types.h>
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
 * Stores calling information per thread.
 *
 * This is necessary as calculating the duration for one function call would be impossible otherwise
 * in a multithreaded environment.
 */
typedef struct per_thread_region_info
{
    /** Thread id the information belong to */
    uint64_t thread_idx;
    /** Timestamp of last enter of the region in this thread */
    uint64_t last_enter;
#ifdef FILTERING_ABSOLUTE
    /** Counter for region entries in this thread */
    uint64_t call_cnt;
    /** Calculated region duration for this thread */
    uint64_t duration;
#endif
    /** Linked list next pointer */
    struct per_thread_region_info* nxt;
#ifdef FILTERING_ABSOLUTE
    /** Marks whether the current thread is fine with deleting the call */
    bool deletable;
#endif
} per_thread_region_info;

/**
 * Stores region info.
 */
typedef struct region_info
{
    /** Handle identifying the region */
    uint64_t region_handle;
    /** Number of threads currently between enter and exit for this region */
    uint64_t active_threads;
    /** Number of threads waiting for deletion */
    uint64_t waiting_threads;
#ifdef FILTERING_RELATIVE
    /** Global counter for region entries */
    uint64_t call_cnt;
    /** Global calculated region duration */
    uint64_t duration;
    /** Mean region duration used for comparison */
    float mean_duration;
#endif
    /** Pointer to the local information */
    per_thread_region_info* local_info;
    /** Linked list next pointer */
    struct region_info* nxt;
    /** Pointer to the callq for the enter instrumentation function */
    char* enter_func;
    /** Conditional variable for deletion synchronization */
    pthread_cond_t cond_var;
    /** Marks whether the region is deletable */
    bool deletable;
} region_info;

/** Global list of defined regions */
region_info* regions = NULL;

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

/** Thread local id */
__thread uint64_t thread_idx = 0;

#ifdef FILTERING_RELATIVE
/** Mean duration across all regions */
float mean_duration = 0;

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
#endif

/**
 * Adds thread local information for a newly created region.
 *
 * Gets called whenever a new region is created.
 *
 * @param   region_info_ptr                 A pointer to the newly created region_info object.
 */
static void add_local_infos( region_info*                               region_info_ptr )
{
    int cntr = 0;
    per_thread_region_info* current = regions->local_info;

    // Count the number of thread infos to build
    while( current != NULL )
    {
        cntr++;
        current = current->nxt;
    }

    per_thread_region_info* last = NULL;
    per_thread_region_info* new;

    // Backwards building the thread local info list
    while( cntr > 0 )
    {
        new = calloc( 1, sizeof( per_thread_region_info ) );
        new->thread_idx = cntr;
        new->nxt = last;

        if( cntr == 1 )
        {
            region_info_ptr->local_info = new;
        }
        else
        {
            last = new;
        }

        cntr--;
    }
}

/**
 * Register a new thread.
 *
 * Gets called whenever a thread tries to access thread local information for the first time.
 */
static void register_thread( )
{
    thread_idx++;
    per_thread_region_info* last = NULL;
    per_thread_region_info* current = regions->local_info;

    pthread_mutex_lock( &mtx );

    while( current != NULL )
    {
        thread_idx++;
        last = current;
        current = current->nxt;
    }

    per_thread_region_info* new;
    region_info* current_region = regions;

    if( last == NULL )
    {
        // We register the first thread
        while( current_region != NULL )
        {
            new = calloc( 1, sizeof( per_thread_region_info ) );
            new->thread_idx = thread_idx;

            current_region->local_info = new;
            current_region = current_region->nxt;
        }
    }
    else
    {
        // We register another thread
        int i;

        while( current_region != NULL )
        {
            current = current_region->local_info;

            // We're on the first element (thread_idx == 1) and want to go to the last existing
            // (our thread_idx - 1)
            for( i = 1; i < thread_idx - 1; ++i )
            {
                current = current->nxt;
            }

            // Create a new element
            new = calloc( 1, sizeof( per_thread_region_info ) );
            new->thread_idx = thread_idx;

            // Swap buffers
            current->nxt = new;
            current_region = current_region->nxt;
        }
    }

    // Unlock write mutex
    pthread_mutex_unlock( &mtx );
}

/**
 * Get internal pointer to region info for given region handle.
 *
 * @param   region_handle                   The region to look up.
 *
 * @return                                  The corresponding region info.
 */
static inline region_info* get_region( uint64_t                         region_handle )
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
 * if the calling thread is already registered and registers it if necessary. Afterwards the correct
 * thread local information for the given region is returned.
 *
 * @param   region_handle                   The region to access the thread local information for.
 *
 * @return                                  The thread local information for the given region and
 *                                          the calling thread.
 */
static per_thread_region_info* get_region_info( uint64_t                region_handle )
{
    // Is this thread already registered?
    if( thread_idx == 0 )
    {
        register_thread( );
    }

    // Now we're sure, get the info
    per_thread_region_info* current = get_region( region_handle )->local_info;

    // Select the correct thread local information
    while( current->thread_idx != thread_idx )
    {
        current = current->nxt;
    }

    return current;
}

/**
 * As long as there's more than one thread, there's no save way to make any changes to their common
 * TXT segment. So we have to notice the creation of threads and wait untill all of them have
 * joined.
 */
static void on_team_begin( struct SCOREP_Location*                      scorep_location,
                           uint64_t                                     timestamp,
                           SCOREP_ParadigmType                          scorep_paradigm,
                           SCOREP_InterimCommunicatorHandle             scorep_thread_team )
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
static void on_team_end( struct SCOREP_Location*                        scorep_location,
                         uint64_t                                       timestamp,
                         SCOREP_ParadigmType                            scorep_paradigm,
                         SCOREP_InterimCommunicatorHandle               scorep_thread_team )
{
    // Thread joining doesn't have to be protected, because if there are threads to join, we'll
    // never get into deletion.
    atomic_fetch_sub( &thread_ctr, 1 );
}

/**
 * Enter region.
 *
 * This updates the region info (call counter and other relevant metrics) and decides whether or
 * not the region instrumentation should be overwritten. If so, it checks if we're in a multi-
 * threaded environment and acts accordingly:
 *
 *  * multithreaded: Skip deletion and wait untill all threads are joined.
 *  * singlethreaded: Mark the state as deletion ready and hold a mutex so that no new spawned
 *    thread can enter a possibly deleted region.
 *
 * For metric calculation at least two methods should be implemented, a relative and an absolute.
 */
static void on_enter( struct SCOREP_Location*                           scorep_location,
                      uint64_t                                          timestamp,
                      SCOREP_RegionHandle                               region_handle,
                      uint64_t*                                         metric_values )
{
    region_info* region = get_region( region_handle );
    per_thread_region_info* info = get_region_info( region_handle );

    pthread_mutex_lock( &mtx );

#if defined( FILTERING_RELATIVE ) || defined( FILTERING_ABSOLUTE )
    // Calculate some metrics.
    printf( "on_enter\n" );
    info->last_enter = timestamp;

#ifdef FILTERING_RELATIVE
    region->call_cnt++;
#endif /* FILTERING_RELATIVE */

#ifdef FILTERING_ABSOLUTE
    info->call_cnt++;
#endif /* FILTERING_ABSOLUTE */
#endif /* FILTERING_RELATIVE || FILTERING_ABSOLUTE */

    if( region->deletable )
    {
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
            // There are other threads around so deletion is impossible. Unlock the mutex and carry
            // on.
            pthread_mutex_unlock( &deletion_barrier );
        }
    }

    pthread_mutex_unlock( &mtx );
}

/**
 * Exit region.
 *
 * This method contains further code for calculating metrics (see on_enter as well) and the actual
 * instrumentation override code.
 */
static void on_exit( struct SCOREP_Location*                            scorep_location,
                     uint64_t                                           timestamp,
                     SCOREP_RegionHandle                                region_handle,
                     uint64_t*                                          metric_values )
{
    region_info* region = get_region( region_handle );
    per_thread_region_info* info = get_region_info( region_handle );

    pthread_mutex_lock( &mtx );

    printf( "on_exit\n" );

    if( atomic_flag_test_and_set( &deletion_ready ) )
    {
        // Reset mutex so normal program flow can be reached again.
        pthread_mutex_unlock( &deletion_barrier );
    }
#if defined( FILTERING_RELATIVE ) || defined( FILTERING_ABSOLUTE )
    else
    {
#ifdef FILTERING_RELATIVE
        region->duration += ( timestamp - info->last_enter );
        region->mean_duration = region->duration / region->call_cnt;
        
        update_mean_duration( );

        if( region->mean_duration < mean_duration - threshold )
        {
            region->deletable = true;
        }
#endif /* FILTERING_RELATIVE */
#ifdef FILTERING_ABSOLUTE
        info->duration += ( timestamp - info->last_enter );

        if( info->duration / info->call_cnt > threshold )
        {
            info->deletable = true;
        }
#endif /* FILTERING_ABSOLUTE */
    }
#endif /* FILTERING_RELATIVE || FILTERING_ABSOLUTE */

    // After atomic_flag_test_and_set deletion_ready is always true whether or not it was before.
    // In neither case we want it to be true after on_exit
    atomic_flag_clear( &deletion_ready );

    pthread_mutex_unlock( &mtx );
}

static void on_define_region( const char*                               region_name,
                              const char*                               region_canonical_name,
                              SCOREP_ParadigmType                       paradigm_type,
                              SCOREP_RegionType                         region_type,
                              SCOREP_RegionHandle                       region_handle )
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
        add_local_infos( regions );
    }
    else
    {
        last->nxt = calloc( 1, sizeof( region_info ) );
        current = last->nxt;
        current->region_handle = region_handle;
        add_local_infos( current );
    }

    pthread_mutex_unlock( &mtx );
}

static void on_init( )
{
    // Set deletion ready to the default false state.
    atomic_flag_clear( &deletion_ready );
    // Get the threshold for filtering
    char* env_str = getenv( "SCOREP_SUBSTRATES_DYNAMIC_FILTERING_THRESHOLD" );
    if( env_str == NULL )
    {
        // TODO report error
    }
    else
    {
        threshold = strtoull( env_str, NULL, 10 );
        perror( "SCOREP_SUBSTRATES_DYNAMIC_FILTERING_THRESHOLD" );
    }
}

static void on_finalize( )
{
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
