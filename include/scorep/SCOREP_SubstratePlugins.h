/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2013,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2013, 2015-2016,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2013,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2013,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2013,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2013,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 *
 */

/**
 * @file       SCOREP_SubstratePlugins.h
 *
 * The developer of a substrate plugin should provide a README file which
 * explains how to compile, install and use the plugin. In particular,
 * the supported substrates should be described in the README file.
 *
 * Each substrate plugin has to include <tt>SCOREP_SubstratePlugins.h</tt>
 * and implement a 'get_info' function. Therefore, use the
 * SCOREP_SUBSTRATE_PLUGIN_ENTRY macro and provide the name of the plugin
 * library as the argument.
 * For example, the substrate plugin libexample_plugin.so should use
 * SCOREP_SUBSTRATE_PLUGIN_ENTRY( example_plugin ).
 * Substrate plugins that implement event functions should also include
 * <tt>SCOREP_SubstrateEvents.h</tt>
 * Plugin writers should also refer to
 * <tt>SCOREP_PublicHandles.h</tt> and <tt>SCOREP_PublicTypes.h</tt>
 * to handle SCOREP handles given in event functions.
 *
 * It is encouraged to use the "_plugin" suffix on the name to avoid
 * conflicts with existing libraries, e.g., libtuning_plugin.so using
 * the existing libtuning.so.
 *
 *
 * @section Functions
 * See each function for details.
 * All functions except early_init are optional!
 *
 * @ref SCOREP_Substrate_Plugin_Info::early_init "early_init"
 *
 * Check requirements and initialize the plugin.
 *
 * @ref SCOREP_Substrate_Plugin_Info::late_init "late_init"
 *
 * Initialization of parts of the plugin that rely on an initialized Score-P.
 *
 * @ref SCOREP_Substrate_Plugin_Info::finalize "finalize"
 *
 * Finalization of Score-P.
 *
 * @ref SCOREP_Substrate_Plugin_Info::create_location "create_location"
 *
 * Create a new location (e.g., right after late init or when a thread is created)
 *
 * @ref SCOREP_Substrate_Plugin_Info::delete_location "delete_location"
 *
 * Delete an existing location
 *
 * @ref SCOREP_Substrate_Plugin_Info::activate_location "activate_location"
 *
 * Activate a location to write events. Called after create_location
 *
 * @ref SCOREP_Substrate_Plugin_Info::deactivate_location "deactivate_location"
 *
 * Activate a location to write events. Called before delete_location
 *
 * @ref SCOREP_Substrate_Plugin_Info::pre_unify "pre_unify"
 *
 * Called before the unify step. After the measurement.
 *
 * @ref SCOREP_Substrate_Plugin_Info::write_data "write_data"
 *
 * Called after the measurement when writing data.
 *
 * @ref SCOREP_Substrate_Plugin_Info::core_task_create "core_task_create"
 *
 * Create a task struct (e.g., an OpenMP task)
 *
 * @ref SCOREP_Substrate_Plugin_Info::core_task_complete "core_task_complete"
 *
 * Complete a task (e.g., an OpenMP task)
 *
 * @ref SCOREP_Substrate_Plugin_Info::define_handle "define_handle"
 *
 * Define a handle (e.g., a string or a region)
 *
 * @ref SCOREP_Substrate_Plugin_Info::get_event_functions "get_event_functions"
 *
 * Called twice after late_init with different modes. Get a list of events that shall be recorded for locations
 *
 * @ref SCOREP_Substrate_Plugin_Info::set_callbacks "set_callbacks"
 *
 * Called after get_event_functions. Set a list of callbacks so that the plugin can get meta data for handles.
 *
 * @ref SCOREP_Substrate_Plugin_Info::reserved "reserved"
 *
 * Must be set to zero!
 */

#ifndef SCOREP_SUBSTRATE_PLUGINS_H
#define SCOREP_SUBSTRATE_PLUGINS_H

#include <stdlib.h>
#include <stddef.h>

#include <scorep/SCOREP_PublicHandles.h>
#include <scorep/SCOREP_SubstrateEvents.h>


/** Current version of Score-P substrate plugin interface */
#define SCOREP_SUBSTRATE_PLUGIN_VERSION 1

#define SCOREP_SUBSTRATE_PLUGIN_UNDEFINED_MANAGEMENT_FUNCTIONS 100

#ifdef __cplusplus
#   define EXTERN extern "C"
#else
#   define EXTERN extern
#endif


/** Macro used for implementation of the 'get_info' function */
#define SCOREP_SUBSTRATE_PLUGIN_ENTRY( _name ) \
    SCOREP_Substrate_Plugin_Info \
    SCOREP_SubstratePlugin_ ## _name ## _get_info( void )

typedef struct SCOREP_SubstrateCallbacks
{
    /**
     * Returns the id of an attribute handle.
     * @param handle to attribute
     * @return id of attribute
     */
    uint32_t
    ( * SCOREP_AttributeHandle_GetId )( SCOREP_AttributeHandle handle );

    /**
     * Returns the type of an attribute handle. (@see SCOREP_PublicTypes.h)
     * @param handle to attribute
     * @return type of attribute
     */
    SCOREP_AttributeType
    ( * SCOREP_AttributeHandle_GetType )( SCOREP_AttributeHandle handle );


    /**
     * Returns the region of a calling context node. (@see SCOREP_PublicTypes.h)
     * @param handle to node
     * @return handle to the region
     */
    SCOREP_RegionHandle
    ( * SCOREP_CallingContextHandle_GetRegion )( SCOREP_CallingContextHandle handle );

    /**
     * Returns the parent of a calling context node.
     * @param handle to node
     * @return handle of parent
     */
    SCOREP_CallingContextHandle
    ( * SCOREP_CallingContextHandle_GetParent )( SCOREP_CallingContextHandle handle );


    /**
     * Returns the value type of a metric. (@see SCOREP_MetricTypes.h)
     * @param handle to local metric definition.
     */
    SCOREP_MetricValueType
    ( * SCOREP_MetricHandle_GetValueType )( SCOREP_MetricHandle handle );


    /**
     * Returns the name of a metric.
     * @param handle to local metric definition.
     */
    const char*
    ( *SCOREP_MetricHandle_GetName )( SCOREP_MetricHandle handle );


    /**
     * Returns the profiling type of a metric. (@see SCOREP_MetricTypes.h)
     * @param handle to local metric definition.
     */
    SCOREP_MetricProfilingType
    ( * SCOREP_MetricHandle_GetProfilingType )( SCOREP_MetricHandle handle );

    /**
     * Returns the mode of a metric. (@see SCOREP_MetricTypes.h)
     * @param handle to local metric definition.
     */
    SCOREP_MetricMode
    ( * SCOREP_MetricHandle_GetMode )( SCOREP_MetricHandle handle );

    /**
     * Returns the source type of a metric.(@see SCOREP_MetricTypes.h)
     * @param handle to local metric definition.
     */
    SCOREP_MetricSourceType
    ( * SCOREP_MetricHandle_GetSourceType )( SCOREP_MetricHandle handle );

    /**
     * Returns the name of a parameter
     * @param handle to parameter
     * @return name
     */
    const char*
    ( *SCOREP_ParameterHandle_GetName )( SCOREP_ParameterHandle handle );


    /**
     * Returns parameter type (@see SCOREP_PublicTypes.h)
     * @param handle to parameter
     * @return type of parameter
     */
    SCOREP_ParameterType
    ( * SCOREP_ParameterHandle_GetType )( SCOREP_ParameterHandle handle );


    /**
     * Returns parameter id
     * @param handle to parameter
     * @return id of parameter
     */
    uint32_t
    ( * SCOREP_RegionHandle_GetId )( SCOREP_RegionHandle handle );

    /**
     * Returns region demangled name
     * @param handle to region
     * @return demangled region name
     */

    const char*
    ( *SCOREP_RegionHandle_GetName )( SCOREP_RegionHandle handle );


    /**
     * Returns regions mangled canonical name
     * @param handle to region
     * @return mangled region name
     */

    const char*
    ( *SCOREP_RegionHandle_GetCanonicalName )( SCOREP_RegionHandle handle );


    /**
     * Returns file name where region is defined
     * @param handle to region
     * @return file name
     */
    const char*
    ( *SCOREP_RegionHandle_GetFileName )( SCOREP_RegionHandle handle );


    /**
     * Returns begin line of a function within the source file
     * @param handle to region
     * @return begin line
     */
    SCOREP_LineNo
    ( * SCOREP_RegionHandle_GetBeginLine )( SCOREP_RegionHandle handle );


    /**
     * Returns end line of a function within the source file
     * @param handle to region
     * @return end line
     */
    SCOREP_LineNo
    ( * SCOREP_RegionHandle_GetEndLine )( SCOREP_RegionHandle handle );

    /**
     * Returns region type (@see SCOREP_PublicTypes.h)
     * @param handle to region
     * @return type (e.g. SCOREP_REGION_USER for user regions)
     */
    SCOREP_RegionType
    ( * SCOREP_RegionHandle_GetType )( SCOREP_RegionHandle handle );


    /**
     * Returns region paradigm (@see SCOREP_PublicTypes.h)
     * @param handle to region
     * @return paradigm (e.g. SCOREP_PARADIGM_MPI for MPI regions)
     */
    SCOREP_ParadigmType
    ( * SCOREP_RegionHandle_GetParadigmType )( SCOREP_RegionHandle handle );


    /**
     * Returns name of a source file
     * @param handle to source file
     * @return name
     */
    const char*
    ( *SCOREP_SourceFileHandle_GetName )( SCOREP_SourceFileHandle handle );

    /**
     * Resolve string handle
     * @param handle of string
     * @return string
     */
    const char*
    ( *SCOREP_StringHandle_Get )( SCOREP_StringHandle handle );


    /**
     * Get the number of metrics in a sampling set.
     * @param handle the handle to the existing sampling set
     * @return the number of associated metrics
     */
    uint8_t ( * SCOREP_SamplingSet_GetNumberOfMetrics )( SCOREP_SamplingSetHandle handle );

    /**
     * Get the metric handles in a sampling set.
     * @param handle the handle to the existing sampling set
     * @return a list of associated metrics. get the length of the list with SCOREP_SamplingSet_GetNunmberOfMetrics
     */
    const SCOREP_MetricHandle* ( *SCOREP_SamplingSet_GetMetricHandles )( SCOREP_SamplingSetHandle handle );


    /**
     * Get the name of a location
     * @param location handle
     * @return location name
     */
    const char*
    ( *SCOREP_Location_GetName )( const struct SCOREP_Location* locationData );

    /**
     * Get a unique global id of a location
     * @param location handle
     * @return global location id
     */
    uint64_t
    ( * SCOREP_Location_GetGlobalId )( const struct SCOREP_Location* locationData );

    /**
     * Get last time stamp of a location
     * @param location handle
     * @return last time stamp set by Score-P
     */
    uint64_t
    ( * SCOREP_Location_GetLastTimestamp )( const struct SCOREP_Location* locationData );

    /**
     *  Returns the LOCAL id of the location (ie. thread id).
     *  (see also SCOREP_Location_GetGlobalId)
     * @return ID that is unique within the process
     */
    uint32_t
    ( * SCOREP_Location_GetId )( const struct SCOREP_Location* locationData );


    /**
     *  Returns the type of the location.
     *
     * @return
     */
    SCOREP_LocationType
    ( * SCOREP_Location_GetType )( const struct SCOREP_Location* locationData );
} SCOREP_SubstrateCallbacks;


/**
 * Defines a Substrate plugin.
 * The call order of these functions is:
 *
 * Called per process:
 * ...Score-P initialization part 1, e.g. environment variables...
 * (0) SCOREP_SUBSTRATE_PLUGIN_ENTRY
 * (1) early_init
 * (2) get_event_functions( SCOREP_SUBSTRATES_RECORDING_ENABLED )
 * (3) get_event_functions( SCOREP_SUBSTRATES_RECORDING_DISABLED )
 * (4) set_callbacks()
 * (A) From now on at any point in time there can be new definitions.
 *     Definitions are unique in a process context. Thus two different locations
 *     can use the same definitions and there are no handles that are only
 *     valid for a specific location. Plugins should care for thread safeness
 *     when registering handles in internal data structures.
 * ...Score-P initialization part 2...
 * (5) assign_id()
 * ...Score-P initialization part 3, e.g. MPP...
 * (5) late_init()
 * Called for each location (e.g., thread)
 * {
 *    create_location()
 *
 *      activate_location()
 *      (optional) deactivate_location() followed by an activate_location()
 *      (optional) core_task_create() optionally followed by events and core_task_complete()
 *      (optional) events to the provided event_functions (see get_event_functions)
 *    deactivate_location()
 *    delete_location()
 * }
 * (N-2) pre_unify()
 * (N-1) write_data()
 * (N) finalize()
 */
typedef struct SCOREP_Substrate_Plugin_Info
{
    /**
     * Should be set to SCOREP_SUBSTRATE_PLUGIN_VERSION
     * (needed for back- and forward compatibility)
     */
    uint32_t plugin_version;

    /**
     * This function is called before most internal Score-P data is initialized.
     * @param substrate_id a specific id that is assigned to the plugin
     * This ID is only valid for the current Score-P run.
     *
     */
    int ( * early_init )();

    /**
     * This function is called before most internal Score-P data is initialized.
     * @param substrate_id a specific ID that is assigned to the plugin
     * This ID is only valid for the current Score-P run.
     * It should be returned with finalize()
     * @ret 0 on success
     */
    void ( * assign_id )( size_t substrate_id );

    /**
     * This function is called after MPP paradigms are initialized.
     * Therefore, it is save to do MPI communication in this and after this call.
     */
    void ( * late_init )( void );

    /**
     * This function is called when the Score-P run finished
     * @return the ID that has been assigned in assign_id()
     */
    size_t ( * finalize )( void );

    /**
     * This function is called whenever a new location is created, e.g.
     * whenever a new OpenMP thread is created.
     *
     * @param location location which is created
     *
     * @param parentLocation location that created this location
     */
    void ( * create_location )( struct SCOREP_Location* location,
                                struct SCOREP_Location* parentLocation );

    /**
     * This function is called whenever a location is activated
     * @see create_location
     *
     * @param location location which is activated
     */
    void ( * activate_cpu_location )( struct SCOREP_Location* location,
                                      struct SCOREP_Location* parentLocation,
                                      uint32_t                forkSequenceCount );

    /**
     * This function is called whenever a location is deactivated
     * @see create_location
     *
     * @param location location which is activated
     */
    void ( * deactivate_cpu_location )( struct SCOREP_Location* location,
                                        struct SCOREP_Location* parentLocation );

    /**
     * This function is called whenever a location is deleted
     *
     * @see create_location
     * @param location location which is deleted
     */
    void ( * delete_location )( struct SCOREP_Location* location );

    /**
     * This function is called before the data about different threads and
     * MPI processes is collected and unified, i.e. when definitions are synchronized.
     */
    void ( * pre_unify  )( void );

    /**
     * This function is called after the unififcation process
     * when traces/profiles are written - right before finalize.
     * This should be used to flush recorded data.
     */
    void ( * write_data )( void );

    /**
     * Called when a task (e.g., an OpenMP task) is creates
     * The taskHandle is not defined via define_handle and can not be converted to SCOREP_AnyHandle
     */
    void ( * core_task_create )( struct SCOREP_Location* locationData,
                                 SCOREP_TaskHandle       taskHandle  );

    void ( * core_task_complete )( struct SCOREP_Location* locationData,
                                   SCOREP_TaskHandle       taskHandle  );

    /**
     * This function will be called whenever a new definition is created
     * Plugins can filter the processing of definitions according to the given type.
     * Plugins should use the methods in <tt>SCOREP_SubstrateAccess.h</tt>,
     * <tt>SCOREP_PublicHandles.h</tt>, and <tt>SCOREP_PublicTypes.h</tt>
     *  to make sense from the handle, (e.g., to get the name of a region)
     */
    void ( * define_handle )( SCOREP_AnyHandle  handle,
                              SCOREP_HandleType type );

    /**
     * Get all functions for events, attributed to their SCOREP_Substrates_EventType
     * These functions are called whenever recording is set to "on" in Score-P
     * @arg functions: a pointer to the functions that are assigned to the types.
     *  The returned array MUST hold SCOREP_SUBSTRATES_NUM_EVENTS elements.
     *  Not-implemented functions should be set to NULL.
     *  The array will not be free'd by Score-P.
     * @ret Return SCOREP_SUBSTRATES_NUM_EVENTS (see <tt>SCOREP_SubstrateEvents.h</tt>)
     */
    uint32_t ( * get_event_functions )( SCOREP_Substrates_Mode       mode,
                                        SCOREP_Substrates_Callback** functions );

    /**
     * Provide plugins with pointers to functions that can be used to get meta data
     * about handles.
     * @param callbacks the provided function callbacks
     * @param sizeof(SCOREP_SubstrateCallbacks) The plugin should care that its version of SCOREP_SubstrateCallbacks is smaller or equal size
     */
    void ( * set_callbacks )( SCOREP_SubstrateCallbacks callbacks,
                              size_t                    size );

    /**
     *  for future extensions
     *  Plugins should set this to 0 (e.g. memset)
     *  When a new function is added SCOREP_SUBSTRATE_PLUGIN_UNDEFINED_MANAGEMENT_FUNCTIONS should be decreased by 1.
     *  Score-P should check for an appropriate plugin_version before calling this new function.
     *  Score-P is not able to check whether this is set to 0 but might use an invalid function.
     *  This can be avoided by using the correct plugin_version.
     */
    uint64_t reserved[ SCOREP_SUBSTRATE_PLUGIN_UNDEFINED_MANAGEMENT_FUNCTIONS ];
} SCOREP_Substrate_Plugin_Info;

#endif /* SCOREP_SUBSTRATE_PLUGINS_H */
