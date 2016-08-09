#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SCOREP_Location.h>
#include <SCOREP_Types.h>
#include <scorep_substrates_definition.h>

#include "delete-call.h"


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
    /** Counter for region entries in this thread */
    uint64_t call_cnt;
    /** Timestamp of last enter of the region in this thread */
    uint64_t last_enter;
    /** Calculated region duration for this thread */
    uint64_t duration;
    /** Marks whether the current thread is fine with deleting the call */
    bool deletable;
    /** Linked list next pointer */
    struct per_thread_region_info* nxt;
} per_thread_region_info;

/**
 * Stores region info.
 */
typedef struct region_info
{
    uint64_t region_handle;
    /** Number of threads currently between enter and exit for this region */
    uint64_t active_threads;
    /** Number of threads waiting for deletion */
    uint64_t waiting_threads;
    /** ID of the thread responsible for deleting the calls */
    uint64_t deleter_id;
    /** Conditional variable for deletion synchronization */
    pthread_cond_t cond_var;
    /** Pointer to the callq for the enter instrumentation function */
    char* enter_func;
    per_thread_region_info* local_info;
    struct region_info* nxt;
} region_info;

/** Global list of defined regions */
region_info* regions = NULL;

/** Mutex used for writing */
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/** Thread local id */
__thread uint64_t thread_idx = 0;


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

    // Free write mutex
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
 * Enter region.
 *
 */
static void on_enter( struct SCOREP_Location*                           scorep_location,
                      uint64_t                                          timestamp,
                      SCOREP_RegionHandle                               region_handle,
                      uint64_t*                                         metric_values )
{
    region_info* region = get_region( region_handle );

    pthread_mutex_lock( &mtx );
    region->active_threads++;
    pthread_mutex_unlock( &mtx );

    // Testing stub
    if( get_region_info( region_handle )->call_cnt++ >= 100
     && region->deleter_id == 0 )
    {
        pthread_mutex_lock( &mtx );
        region->deleter_id = thread_idx;
        region->enter_func = get_function_call_ip( "__cyg_profile_func_enter" );
        pthread_cond_init( &( region->cond_var ), NULL );
        pthread_mutex_unlock( &mtx );
    }
}

static void on_exit( struct SCOREP_Location*                            scorep_location,
                     uint64_t                                           timestamp,
                     SCOREP_RegionHandle                                region_handle,
                     uint64_t*                                          metric_values )
{
    printf( "on_exit\n" );
    region_info* region = get_region( region_handle );

    if( region->deleter_id == 0 )
    {
        pthread_mutex_lock( &mtx );
        region->active_threads--;
        pthread_mutex_unlock( &mtx );
    }
    if( region->deleter_id == thread_idx )
    {
        pthread_mutex_lock( &mtx );
        while( region->waiting_threads < region->active_threads )
        {
            pthread_cond_wait( &( region->cond_var ), &mtx );
        }
        override_callq( region->enter_func );
        override_callq( get_function_call_ip( "__cyg_profile_func_exit" ) );
        pthread_mutex_unlock( &mtx );
    }
    else
    {
        pthread_mutex_lock( &mtx );
        region->waiting_threads++;
        pthread_cond_signal( &( region->cond_var ) );
        pthread_mutex_unlock( &mtx );
    }
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
    printf( "In on_init\n" );
}

static void on_finalize( )
{
    printf( "In on_finalize\n" );
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
