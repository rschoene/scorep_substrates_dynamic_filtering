#include <pthread.h>
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
    /** Linked list next pointer */
    struct per_thread_region_info* nxt;
} per_thread_region_info;

/**
 * Stores region info.
 */
typedef struct region_info
{
    uint64_t region_handle;
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

    pthread_mutex_lock( &mtx );

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
        new = malloc( sizeof( per_thread_region_info ) );
        new->thread_idx = cntr;
        new->call_cnt = 0;
        new->last_enter = 0;
        new->duration = 0;
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

    pthread_mutex_unlock( &mtx );
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
            new = malloc( sizeof( per_thread_region_info ) );
            new->thread_idx = thread_idx;
            new->call_cnt = 0;
            new->last_enter = 0;
            new->duration = 0;

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
            new = malloc( sizeof( per_thread_region_info ) );
            new->thread_idx = thread_idx;
            new->call_cnt = 0;
            new->last_enter = 0;
            new->duration = 0;
            new->nxt = NULL;

            // Swap buffers
            current->nxt = new;
            current_region = current_region->nxt;
        }
    }

    // Free write mutex
    pthread_mutex_unlock( &mtx );
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
    region_info* current_region = regions;

    // Select the correct region
    while( current_region->region_handle != region_handle )
    {
        current_region = current_region->nxt;
    }

    per_thread_region_info* current = current_region->local_info;

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
    per_thread_region_info* info = get_region_info( region_handle );
    info->call_cnt++;
    printf( "call_cnt (thread %d): %d\n", thread_idx, info->call_cnt );
}

static void on_exit( struct SCOREP_Location*                            scorep_location,
                     uint64_t                                           timestamp,
                     SCOREP_RegionHandle                                region_handle,
                     uint64_t*                                          metric_values )
{
}

static void on_define_region( const char*                               region_name,
                              const char*                               region_canonical_name,
                              SCOREP_ParadigmType                       paradigm_type,
                              SCOREP_RegionType                         region_type,
                              SCOREP_RegionHandle                       region_handle )
{
    region_info* last = NULL;
    region_info* current = regions;

    while( current != NULL )
    {
        last = current;
        current = current->nxt;
    }

    if( last == NULL )
    {
        regions = malloc( sizeof( region_info ) );
        regions->region_handle = region_handle;
        regions->local_info = NULL;
        regions->nxt = NULL;
        add_local_infos( regions );
    }
    else
    {
        last->nxt = malloc( sizeof( region_info ) );
        current = last->nxt;
        current->region_handle = region_handle;
        current->local_info = NULL;
        current->nxt = NULL;
        add_local_infos( current );
    }
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
