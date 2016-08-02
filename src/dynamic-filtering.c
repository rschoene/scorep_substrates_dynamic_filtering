#include <stdlib.h>
#include <stdio.h>

#include <SCOREP_Types.h>
#include <scorep_substrates_definition.h>

#include "delete-call.h"


static void on_enter( struct SCOREP_Location*       scorep_location,
                      uint64_t                      timestamp,
                      SCOREP_RegionHandle           region_handle,
                      uint64_t*                     metric_values )
{
    printf( "In on_enter\n" );
    //delete_call( "__cyg_profile_func_enter" );
}

static void on_exit( struct SCOREP_Location*        scorep_location,
                     uint64_t                       timestamp,
                     SCOREP_RegionHandle            region_handle,
                     uint64_t*                      metric_values )
{
    printf( "In on_exit\n" );
    //delete_call( "__cyg_profile_func_exit" );
}

static void on_define_region( const char*           region_name,
                              const char*           region_canonical_name,
                              SCOREP_ParadigmType   paradigm_type,
                              SCOREP_RegionType     region_type,
                              SCOREP_RegionHandle   region_handle )
{

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
