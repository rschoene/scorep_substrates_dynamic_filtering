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
 * @brief Description of the substrate plugin header.
 *        For information on how to use substrate plugins, please refer to section @secref{substrate_plugins}.
 */

#ifndef SCOREP_SUBSTRATE_PLUGINS_H
#define SCOREP_SUBSTRATE_PLUGINS_H

/**
 * @section advice_substrate_plugins Advice for developers
 * The developer of a substrate plugin should provide a README file which
 * explains how to compile, install and use the plugin. In particular,
 * the supported substrates should be described in the README file.
 *
 * Each substrate plugin has to include <tt>SCOREP_SubstratePlugins.h</tt>
 * and implement a 'get_info' function. Therefore, use the
 * #SCOREP_SUBSTRATE_PLUGIN_ENTRY macro and provide the name of the plugin
 * library as the argument.
 * The plugin library must be called libscorep_substrate_<libraryname>.so
 * For example, the example substrate plugin libscorep_substrate_example.so should use
 * #SCOREP_SUBSTRATE_PLUGIN_ENTRY( example ).
 * Substrate plugins that implement event functions should also include
 * <tt>SCOREP_SubstrateEvents.h</tt>
 * Plugin writers should also refer to
 * <tt>SCOREP_PublicHandles.h</tt> and <tt>SCOREP_PublicTypes.h</tt>
 * to handle SCOREP handles given in event functions.
 *
 * @section funcs Functions
 * See each function for details.
 * All functions except init are optional!
 *
 * @ref SCOREP_SubstratePluginInfo::init "init"
 *
 * Check requirements and initialize the plugin.
 *
 * @ref SCOREP_SubstratePluginInfo::assign_id "assign_id"
 *
 * The plugin gets an id that can be used later to store location specific data.
 *
 * @ref SCOREP_SubstratePluginInfo::init_mpp "init_mpp"
 *
 * If an MPP paradigm is used, it will be initialized when this call occurs.
 * If no MPP paradigm is used, this function will be called as well.
 *
 * @ref SCOREP_SubstratePluginInfo::finalize "finalize"
 *
 * Finalization of Score-P.
 *
 * @ref SCOREP_SubstratePluginInfo::create_location "create_location"
 *
 * Create a new location (e.g., when a thread is created)
 *
 * @ref SCOREP_SubstratePluginInfo::delete_location "delete_location"
 *
 * Delete an existing location
 *
 * @ref SCOREP_SubstratePluginInfo::activate_cpu_location "activate_cpu_location"
 *
 * Activate a CPU location to write events. Called, for example, after create_location on CPU locations.
 *
 * @ref SCOREP_SubstratePluginInfo::deactivate_cpu_location "deactivate_cpu_location"
 *
 * Deactivate a CPU location. Called, for example, before delete_location.
 *
 * @ref SCOREP_SubstratePluginInfo::pre_unify "pre_unify"
 *
 * Called before the unify step, after the measurement.
 *
 * @ref SCOREP_SubstratePluginInfo::write_data "write_data"
 *
 * Called after the measurement when writing data.
 *
 * @ref SCOREP_SubstratePluginInfo::core_task_create "core_task_create"
 *
 * Create a task (e.g., an OpenMP task)
 *
 * @ref SCOREP_SubstratePluginInfo::core_task_complete "core_task_complete"
 *
 * Complete a task (e.g., an OpenMP task)
 *
 * @ref SCOREP_SubstratePluginInfo::new_definition_handle "new_definition_handle"
 *
 * Called when a handle is defined, which could define, for example a region or a metric. Plugins can use callbacks to get meta data for this handle.
 *
 * @ref SCOREP_SubstratePluginInfo::get_event_functions "get_event_functions"
 *
 * Called twice with different modes. Get a list of events that shall be passed to the plugin.
 *
 * @ref SCOREP_SubstratePluginInfo::set_callbacks "set_callbacks"
 *
 * Called before get_event_functions. Set a list of callbacks so that the plugin can get meta data for handles.
 *
 * @ref SCOREP_SubstratePluginInfo::undeclared "undeclared"
 *
 * MUST be set to zero. Added for extendability.
 *
 * @section variables Mandatory variable
 * @ref SCOREP_SubstratePluginInfo::plugin_version "plugin_version"
 *
 * Must be set to SCOREP_SUBSTRATE_PLUGIN_VERSION
 */

#include <stdlib.h>
#include <stddef.h>

#include <scorep/SCOREP_PublicHandles.h>
#include <scorep/SCOREP_SubstrateEvents.h>


/** Current version of Score-P substrate plugin interface */
#define SCOREP_SUBSTRATE_PLUGIN_VERSION 1

/** This should be reduced by 1 for each new function added to SCOREP_SubstratePluginInfo */
#define SCOREP_SUBSTRATE_PLUGIN_UNDEFINED_MANAGEMENT_FUNCTIONS 100


#ifdef __cplusplus
#   define EXTERN extern "C"
#else
#   define EXTERN extern
#endif

/** Macro used for implementation of the 'get_info' function */
#define SCOREP_SUBSTRATE_PLUGIN_ENTRY( _name ) \
    EXTERN SCOREP_SubstratePluginInfo \
    SCOREP_SubstratePlugin_ ## _name ## _get_info( void )

/**
 * Callbacks that are passed to Substrate plugins via the set_callbacks(...) call.
 * These callbacks can be used by the plugins to access Score-P internal data and functionality.
 */
typedef struct SCOREP_SubstratePluginCallbacks
{
    /**
     * Returns the Score-P experiment directory.
     * This should be used to write debug/performance data and is available at and after init_mpp is called.
     * Data should be placed under SCOREP_GetExperimentDirName()/(plugin-name)/.
     * Per location data should be placed under SCOREP_GetExperimentDirName()/(plugin-name)/(prefix)(SCOREP_Location_GetGlobalId(location))(suffix).
     * If you want to use the experiment directory, you have to set the SCOREP_SUBSTRATES_REQUIREMENT_EXPERIMENT_DIRECTORY flag in the requirements, see SCOREP_SubstratePluginInfo.get_requirement
     * The name is temporary and the directory might be renamed in the finalization stage.
     */
    const char* ( *SCOREP_GetExperimentDirName )( void );

    /**
     *  Get the number of processes in this parallel program.
     *  Can be called after Plugin receives init_mpp() call.
     *
     * @return If MPP paradigm is used we get the total number of processes that participate in the MPP paradigm, if no MPP paradigm is used return 1
     */
    int
    ( * SCOREP_Ipc_GetSize )( void );


    /**
     * Get the rank of the process.
     * Can be called after Plugin receives init_mpp() call.
     *
     * @return If MPP paradigm is used get an identifier within the MPP paradigm (e.g., an MPI rank). If no MPP paradigm is used return 0
     */
    int
    ( * SCOREP_Ipc_GetRank )( void );

    /**
     * Send data do a specific process (blocking) within the MPP paradigm.
     * Can be called after Plugin receives init_mpp() call.
     * Should not be called if no MPP paradigm is used.
     *
     * @param bufpointer to the buffer of the data that should be sent
     * @param count number of elements in buffer
     * @param datatype type of data
     * @param dest rank of the receiver process (must be smaller than SCOREP_Ipc_GetSize() and different from SCOREP_Ipc_GetRank() )
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Send )(   const void*         buf,
                             int                 count,
                             SCOREP_Ipc_Datatype datatype,
                             int                 dest );


    /**
     * Receive data from a specific process (blocking)
     * Can be called after Plugin receives init_mpp() call.
     * Should not be called if no MPP paradigm is used.
     *
     * @param buf pointer to buffer there the received data should be stored
     * @param count number of elements in buffer
     * @param datatype type of data
     * @param source rank of the source process (must be smaller than SCOREP_Ipc_GetSize() and different from SCOREP_Ipc_GetRank() )
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Recv )(   void*               buf,
                             int                 count,
                             SCOREP_Ipc_Datatype datatype,
                             int                 source );


    /**
     * Wait until every process that is part of the MPP paradigm entered the barrier, otherwise return.
     * Can be called after Plugin receives init_mpp() call.
     *
     * @return return zero on success and something else if an error occurred
     */

    int
    ( * SCOREP_Ipc_Barrier )( void );


    /**
     * Send data to every process within the MPP paradigm (if MPP paradigm is used), including self.
     * Can be called after Plugin receives init_mpp() call.
     *
     * @param buf pointer to the buffer of the data that should be sent
     * @param count number of elements in buffer
     * @param datatype type of data
     * @param root rank of the source process
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Bcast )(  void*               buf,
                             int                 count,
                             SCOREP_Ipc_Datatype datatype,
                             int                 root );

    /**
     * Gathers data from every process within the MPP paradigm (if MPP paradigm is used),
     * root included, with an equal amount of sent for each process.
     *
     * Can be called after Plugin receives init_mpp() call.
     *
     * @param sendbuf pointer to the buffer of the data that should be sent
     * @param recvbuf pointer to buffer there the received data should be stored
     *        data is stored in rank order
     *        size of the buffer should be big enough for the data of all processes
     *        (SCOREP_Ipc_GetSize()*sizeof(type(datatype)))
     * @param count number of elements in buffer
     * @param datatype type of data
     * @param root rank of the process, that should receive all the data
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Gather )( const void*         sendbuf,
                             void*               recvbuf,
                             int                 count,
                             SCOREP_Ipc_Datatype datatype,
                             int                 root );


    /**
     * Gathers data from every process, root included, with varying amount of
     * sent data from each process.
     * Can be called after Plugin receives init_mpp() call.
     *
     * @param sendbuf pointer to the buffer of the data that should be sent
     * @param sendcount number of elements in sent buffer
     * @param recvbuf pointer to buffer there the received data should be stored
     *        data is stored in rank order
     *        size of the buffer should be big enough for the data of all processes
     * @param recvcnts array with the number of elements that should be received from
     *        each process
     *        length should be the number of process
     * @param datatype type of data
     * @param root rank of the process, that should receive all the data
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Gatherv )(    const void*         sendbuf,
                                 int                 sendcount,
                                 void*               recvbuf,
                                 const int*          recvcnts,
                                 SCOREP_Ipc_Datatype datatype,
                                 int                 root );


    /**
     * Gathers data from every process with an equal amount of sent and received
     * data from each process and distributes it to all processes.
     * Can be called after Plugin receives init_mpp() call.
     *
     * @param sendbuf pointer to the buffer of the data that should be sent
     * @param recvbuf pointer to buffer there the received data should be stored
     *        data is stored in rank order
     *        size of the buffer should be big enough for the data of all processes
     * @param count number of elements in buffer
     * @param datatype type of data
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Allgather )(  const void*         sendbuf,
                                 void*               recvbuf,
                                 int                 count,
                                 SCOREP_Ipc_Datatype datatype );

    /**
     * Perform a reduce operation (such as sum, max, logical AND, etc.) with the
     * combined data from every process.
     * Can be called after Plugin receives init_mpp() call.
     *
     * @param sendbuf pointer to the buffer of the data that should be sent
     * @param recvbuf pointer to buffer there the evaluated data should be stored
     * @param count number of elements in buffer
     * @param datatype type of data
     * @param operation operation that should be performed
     * @param root rank of the process, that should receive the evaluated data
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Reduce )( const void*          sendbuf,
                             void*                recvbuf,
                             int                  count,
                             SCOREP_Ipc_Datatype  datatype,
                             SCOREP_Ipc_Operation operation,
                             int                  root );


    /**
     * Perform a reduce operation (such as sum, max, logical AND, etc.) with the
     * combined data from every process and distributes the result to all processes.
     * Can be called after Plugin receives init_mpp() call.
     *
     * @param sendbuf pointer to the buffer of the data that should be sent
     * @param recvbuf pointer to buffer there the evaluated data should be stored
     * @param count number of elements in buffer
     * @param datatype type of data
     * @param operation operation that should be performed
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Allreduce )(  const void*          sendbuf,
                                 void*                recvbuf,
                                 int                  count,
                                 SCOREP_Ipc_Datatype  datatype,
                                 SCOREP_Ipc_Operation operation );

    /** Send data to each process, root included with an equal amount of received
     * data for each process.
     * Can be called after Plugin receives init_mpp() call.
     *
     * @param sendbuf pointer to the buffer of the data that should be sent
     * @param recvbuf pointer to buffer there the received data should be stored
     * @param count number of elements in buffer
     * @param datatype type of data
     * @param root rank of the source process
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Scatter )(    const void*         sendbuf,
                                 void*               recvbuf,
                                 int                 count,
                                 SCOREP_Ipc_Datatype datatype,
                                 int                 root );


    /** Send data in parts to each process, root included with a varying amount
     * of received data for each process.
     * Can be called after Plugin receives init_mpp() call.
     *
     * @param sendbuf pointer to the buffer of the data that should be sent
     * @param sendcounts array with the number of elements that should be sent to each process
     *        length should be the number of process
     * @param recvbuf pointer to buffer there the received data should be stored
     * @param recvcount number of elements in received buffer
     * @param datatype type of data
     * @param root rank of the source process
     *
     * @return return zero on success and something else if an error occurred
     */
    int
    ( * SCOREP_Ipc_Scatterv )(   const void*         sendbuf,
                                 const int*          sendcounts,
                                 void*               recvbuf,
                                 int                 recvcount,
                                 SCOREP_Ipc_Datatype datatype,
                                 int                 root );
    /**
     *  Returns the type of the location.
     *
     * @return
     */
    SCOREP_LocationType
    ( * SCOREP_Location_GetType )( const struct SCOREP_Location* locationData );

    /**
     * Get the name of a location
     * @param location handle
     * @return location name
     */
    const char*
    ( *SCOREP_Location_GetName )( const struct SCOREP_Location* locationData );


    /**
     * Returns the LOCAL id of the location.
     * (see also SCOREP_Location_GetGlobalId)
     * @param location location given from an event, or a location function from SCOREP_SubstratePluginInfo
     * @return ID that is unique within the process
     */
    uint32_t
    ( * SCOREP_Location_GetId )( const struct SCOREP_Location* locationData );

    /**
     * Get a unique global id of a location
     * This is only to be used after init_mpp() is called.
     * @param location location given from an event
     * @return global location id
     */
    uint64_t
    ( * SCOREP_Location_GetGlobalId )( const struct SCOREP_Location* locationData );

    /**
     * Set location private data for a specific location (see SCOREP_Location_GetData)
     * It is save to use it after the location is created (SCOREP_SubstratePluginInfo.create_location) and before the
     * location is deleted (SCOREP_SubstratePluginInfo.delete_location)
     * @param location handle of the location
     * @param plugin_id the id assigned by assign_id
     * @param the data for this location and this plugin
     */
    void
    ( * SCOREP_Location_SetData )( const struct SCOREP_Location* locationData,
                                   size_t                        plugin_id,
                                   void*                         data );

    /**
     * Get location private data for a specific location (see SCOREP_Location_SetData)
     * It is save to use it after the location is created (SCOREP_SubstratePluginInfo.create_location) and before the
     * location is deleted (SCOREP_SubstratePluginInfo.delete_location)
     * @param location handle of the location
     * @param plugin_id the id assigned by assign_id
     * @return the data for this location and this plugin
     */
    void*
    ( *SCOREP_Location_GetData )( const struct SCOREP_Location* locationData,
                                  size_t plugin_id );

    /**
     * Returns the region of a calling context node. (see SCOREP_PublicTypes.h)
     * @param handle handle of the calling context node
     * @return handle to the region that holds the node
     */
    SCOREP_RegionHandle
    ( * SCOREP_CallingContextHandle_GetRegion )( SCOREP_CallingContextHandle handle );

    /**
     * Returns the parent of a calling context node.
     * @param handle handle of the calling context node
     * @return parent handle
     */
    SCOREP_CallingContextHandle
    ( * SCOREP_CallingContextHandle_GetParent )( SCOREP_CallingContextHandle handle );

    /**
     * Returns the value type of a metric.
     * @param handle handle of the local metric definition.
     * @return value type of a metric.
     * @see SCOREP_MetricTypes.h
     */
    SCOREP_MetricValueType
    ( * SCOREP_MetricHandle_GetValueType )( SCOREP_MetricHandle handle );

    /**
     * Returns the name of a metric.
     * @param handle handle of the local metric definition.
     * @return name of a metric.
     */
    const char*
    ( *SCOREP_MetricHandle_GetName )( SCOREP_MetricHandle handle );

    /**
     * Returns the profiling type of a metric.
     * @param handle handle of the local metric definition.
     * @return profiling type of a metric.
     * @see SCOREP_MetricTypes.h
     */
    SCOREP_MetricProfilingType
    ( * SCOREP_MetricHandle_GetProfilingType )( SCOREP_MetricHandle handle );

    /**
     * Returns the mode of a metric.
     * @param handle handle of the local metric definition.
     * @return mode of a metric.
     * @see SCOREP_MetricTypes.h
     */
    SCOREP_MetricMode
    ( * SCOREP_MetricHandle_GetMode )( SCOREP_MetricHandle handle );

    /**
     * Returns the source type of a metric.
     * @param handle handle of the local metric definition.
     * @return source type of a metric.
     * @see SCOREP_MetricTypes.h
     */
    SCOREP_MetricSourceType
    ( * SCOREP_MetricHandle_GetSourceType )( SCOREP_MetricHandle handle );

    /**
     * Returns paradigm class
     * @param handle handle of the paradigm
     * @return class (e.g. SCOREP_PARADIGM_CLASS_MPP for MPI paradigm)
     * @see SCOREP_PublicTypes.h
     */
    SCOREP_ParadigmClass
    ( * SCOREP_ParadigmHandle_GetClass )( SCOREP_ParadigmHandle handle );

    /**
     * Returns paradigm name
     * @param handle handle of the paradigm
     * @return name (e.g., "MPI")
     */
    const char*
    ( *SCOREP_ParadigmHandle_GetName )( SCOREP_ParadigmHandle handle );

    /**
     * Returns paradigm type
     * @param handle handle of the paradigm
     * @return type (e.g. SCOREP_PARADIGM_MPI for MPI)
     * @see SCOREP_PublicTypes.h
     */
    SCOREP_ParadigmType
    ( * SCOREP_ParadigmHandle_GetType )( SCOREP_ParadigmHandle handle );

    /**
     * Returns the name of a parameter
     * @param handle handle of the parameter
     * @return name
     */
    const char*
    ( *SCOREP_ParameterHandle_GetName )( SCOREP_ParameterHandle handle );


    /**
     * Returns parameter type
     * @param handle handle of the parameter
     * @return type of parameter
     * @see SCOREP_PublicTypes.h
     */
    SCOREP_ParameterType
    ( * SCOREP_ParameterHandle_GetType )( SCOREP_ParameterHandle handle );


    /**
     * Returns parameter id
     * @param handle handle of the parameter
     * @return id of parameter
     */
    uint32_t
    ( * SCOREP_RegionHandle_GetId )( SCOREP_RegionHandle handle );

    /**
     * Returns region demangled name
     * @param handle handle of the region
     * @return demangled region name
     */
    const char*
    ( *SCOREP_RegionHandle_GetName )( SCOREP_RegionHandle handle );

    /**
     * Returns regions mangled canonical name
     * @param handle of the region
     * @return mangled region name
     */
    const char*
    ( *SCOREP_RegionHandle_GetCanonicalName )( SCOREP_RegionHandle handle );

    /**
     * Returns file name where region is defined
     * @param handle handle of the region
     * @return file name
     */
    const char*
    ( *SCOREP_RegionHandle_GetFileName )( SCOREP_RegionHandle handle );

    /**
     * Returns begin line of a function within the source file
     * @param handle handle of the region
     * @return begin line
     */
    SCOREP_LineNo
    ( * SCOREP_RegionHandle_GetBeginLine )( SCOREP_RegionHandle handle );

    /**
     * Returns end line of a function within the source file
     * @param handle handle of the region
     * @return end line
     */
    SCOREP_LineNo
    ( * SCOREP_RegionHandle_GetEndLine )( SCOREP_RegionHandle handle );

    /**
     * Returns region type
     * @param handle handle of the region
     * @return type (e.g. SCOREP_REGION_USER for user regions)
     * @see SCOREP_PublicTypes.h
     */
    SCOREP_RegionType
    ( * SCOREP_RegionHandle_GetType )( SCOREP_RegionHandle handle );

    /**
     * Returns region paradigm
     * @param handle handle of the region
     * @return paradigm (e.g. SCOREP_PARADIGM_MPI for MPI regions)
     * @see SCOREP_PublicTypes.h
     */
    SCOREP_ParadigmType
    ( * SCOREP_RegionHandle_GetParadigmType )( SCOREP_RegionHandle handle );

    /**
     * Get the number of metrics in a sampling set.
     * @param handle handle of the the existing sampling set
     * @return the number of associated metrics
     */
    uint8_t ( * SCOREP_SamplingSetHandle_GetNumberOfMetrics )( SCOREP_SamplingSetHandle handle );

    /**
     * Get the metric handles in a sampling set.
     * @param handle the handle of the the existing sampling set
     * @return a list of associated metrics. get the length of the list with SCOREP_SamplingSet_GetNunmberOfMetrics
     */
    const SCOREP_MetricHandle* ( *SCOREP_SamplingSetHandle_GetMetricHandles )( SCOREP_SamplingSetHandle handle );

    /**
     * Get the metric occurrence of a sampling set.
     * @param handle the handle of the the existing sampling set
     * @return the occurrence of handle.
     */
    SCOREP_MetricOccurrence ( * SCOREP_SamplingSetHandle_GetMetricOccurrence )( SCOREP_SamplingSetHandle handle );

    /**
     * Check whether a sampling set is scoped (belongs to a number of locations).
     * @param handle the handle of the the existing sampling set
     * @return whether the sampling set is scoped or not
     */
    bool ( * SCOREP_SamplingSetHandle_IsScoped )( SCOREP_SamplingSetHandle handle );

    /**
     * Returns the scope of the sampling set or SCOREP_INVALID_METRIC_SCOPE if sampling set is not scoped
     * @param handle the handle of the the existing sampling set
     * @return scope or (>=SCOREP_INVALID_METRIC_SCOPE) if the sampling set is not scoped or the runtime version of Score-P is newer than the Score-P version when compiling plugin
     */
    SCOREP_MetricScope ( * SCOREP_SamplingSetHandle_GetScope )( SCOREP_SamplingSetHandle handle );

    /**
     * Returns the class of the sampling set
     * @param handle the handle of the the existing sampling set
     * @return sampling set class
     */
    SCOREP_SamplingSetClass ( * SCOREP_SamplingSetHandle_GetSamplingSetClass )( SCOREP_SamplingSetHandle handle );

    /**
     * Returns name of a source file
     * @param handle handle of the source file
     * @return name
     */
    const char*
    ( *SCOREP_SourceFileHandle_GetName )( SCOREP_SourceFileHandle handle );

    /**
     * Resolve string handle
     * @param handle handle of string
     * @return string
     */
    const char*
    ( *SCOREP_StringHandle_Get )( SCOREP_StringHandle handle );
} SCOREP_SubstratePluginCallbacks;


/**
 * Describes a Substrate plugin. The plugin definition should be done using the SCOREP_SUBSTRATE_PLUGIN_ENTRY macro
 * The call order of these functions is:
 * - Called per process
 *   -# (...Score-P initialization part 1, e.g. environment variables...)
 *   -# resolving of SCOREP_SUBSTRATE_PLUGIN_ENTRY, e.g., for SCOREP_SUBSTRATE_PLUGINS=foo check whether there is a library called libscorep_substrate_foo.so that provides a SCOREP_SUBSTRATE_PLUGIN_ENTRY(foo) / holds the function SCOREP_SubstratePlugin_foo_get_info
 *   -# init()
 *   -# set_callbacks()
 *   -# get_event_functions( ) with mode == SCOREP_SUBSTRATES_RECORDING_ENABLED
 *   -# get_event_functions( ) with mode == SCOREP_SUBSTRATES_RECORDING_DISABLED
 *   -# From now on at any point in time there can be new definitions created via new_definition_handle().
 *      Definitions are unique in a process context. Thus, two different locations within a process
 *      can use the same definitions and there are no handles that are only
 *      valid for a specific location. Plugins should care for thread safeness
 *      when registering handles in internal data structures.
 *   -# From now on at any point in time there can be calls to get_requirement( )
 *   -# (...Score-P initialization part 2...)
 *   -# assign_id()
 *   -# create_location() for the main thread
 *   -# As soon as the MPP paradigm is initialized, init_mpp() is called.
 *      If the program does not use MPP paradigms, init_mpp() will also be called
 *   -# (...Score-P initialization part 3...)
 *   -# Called for each location (e.g., thread)
 *     -# create_location() (Only new locations, remember that the main thread location is already created earlier!)
 *     -# if it is a CPU location: activate_cpu_location()
 *     -# Now there is a sequence of these events
 *       - if it is a CPU location:
 *         - (optional) calls to deactivate_cpu_location() followed by calls to activate_cpu_location()
 *         - (optional) core_task_create(), possibly followed by runtime events, followed by core_task_complete()
 *       - (optional) runtime events
 *     -# if it is a CPU location: deactivate_cpu_location()
 *     -# delete_location()
 *   -# pre_unify()
 *   -# write_data()
 *   -# finalize()
 *
 * Not implemented functions MUST point to NULL, e.g., info.assign_id = NULL;
 */
typedef struct SCOREP_SubstratePluginInfo
{
    /**
     * Must be set to SCOREP_SUBSTRATE_PLUGIN_VERSION
     * (needed for back- and forward compatibility)
     */
    uint32_t plugin_version;

    /**
     * This function is called before most internal Score-P data is initialized.
     * The plugin should be initialized here and dependencies should be checked.
     * @return 0 if initialization succeeded, otherwise !=0
     */
    int ( * init )( void );

    /**
     * This function assigns a specific ID to the plugin that can be used for accessing thread local storage.
     * However, most of the internal functionality is not available at the time this function is called. Therefore, only the id should be stored for now.
     *
     * The function might be called multiple times when the Online Access interface is used and re-initializes Score-P.
     *
     * @param plugin_id a specific ID that is assigned to the plugin and can be used to access thread local storages.
     * (see also SCOREP_Location_SetData and SCOREP_Location_GetData)
     * This ID is only valid for the current Score-P run.
     */
    void ( * assign_id )( size_t pluginId );

    /**
     * This function is called after MPP paradigms are initialized.
     * If the program does not use MPP paradigms this function is also called.
     * To detect used paradigms check for calls to new_definition_handle with type == SCOREP_HANDLE_TYPE_PARADIGM.
     */
    void ( * init_mpp )( void );

    /**
     * This function is called when the Score-P run finished
     */
    void ( * finalize )( void );

    /**
     *  The location callbacks notify the subsystem about the lifetime of a
     *  location. For CPU locations:
     *
     *  - create_location
     *    - initial activate_cpu_location()
     *    - (optional) interim deactivate_cpu_location(), followed by activate_cpu_location()
     *    - final deactivate_cpu_location()
     *  - delete_location
     *
     *  For none-CPU locations:
     *
     *  1. create_location
     *  2. delete_location
     */

    /**
     * This function is called whenever a new location is created, e.g.
     * whenever a new OpenMP thread is created.
     *
     * @param location location which is created
     * @param parentLocation location that created this location
     */
    void ( * create_location )( const struct SCOREP_Location* location,
                                const struct SCOREP_Location* parentLocation );

    /**
     * This function is called whenever a location is activated.
     * @see create_location
     * @see deactivate_cpu_location
     * @see delete_location
     *
     * @param location location which is activated
     * @param parentLocation parent of location. May be equal @a locationData.
     * @param forkSequenceCount an increasing number which defines the current fork from parentLocation. At each fork (or omp parallel) this increases by 1.
     */
    void ( * activate_cpu_location )( const struct SCOREP_Location* location,
                                      const struct SCOREP_Location* parentLocation,
                                      uint32_t                      forkSequenceCount );

    /**
     * This function is called whenever a location is deactivated
     * @see create_location
     * @see activate_cpu_location
     *
     * @param location location which is deactivated
     * @param parentLocation parent of location. May be equal @a locationData.
     */
    void ( * deactivate_cpu_location )( const struct SCOREP_Location* location,
                                        const struct SCOREP_Location* parentLocation );

    /**
     * This function is called whenever a location is deleted
     *
     * @see create_location
     * @param location location which is deleted
     */
    void ( * delete_location )( const struct SCOREP_Location* location );

    /**
     * This function is called before the data about different threads and
     * MPI processes is collected and unified, i.e. when definitions are synchronized.
     */
    void ( * pre_unify  )( void );

    /**
     * This function is called after the unification process
     * when traces/profiles are written - right before finalize.
     * This should be used to flush recorded data.
     */
    void ( * write_data )( void );

    /**
     * Called when a task (e.g., an OpenMP task) is creates
     * The taskHandle is not defined via define_handle and can not be converted to SCOREP_AnyHandle
     * @param location the location that created the task
     * @param taskHandle the newly created task
     */
    void ( * core_task_create )( const struct SCOREP_Location* location,
                                 SCOREP_TaskHandle             taskHandle  );


    /**
     * Called when a task (e.g., an OpenMP task) is completed
     * The taskHandle is not defined via define_handle and can not be converted to SCOREP_AnyHandle
     * @param location the location that completes the task
     * @param taskHandle the completed created task
     */
    void ( * core_task_complete )( const struct SCOREP_Location* location,
                                   SCOREP_TaskHandle             taskHandle  );

    /**
     * This function will be called whenever a new definition is created
     * Plugins can filter the processing of definitions according to the given type.
     * Plugins should use the callbacks passed by set_callbacks,
     * <tt>SCOREP_PublicHandles.h</tt>, and <tt>SCOREP_PublicTypes.h</tt>
     *  to make sense from the handle, (e.g., to get the name of a region)
     *  @param handle a handle to the newly created object
     *  @param type the type of the handle
     */
    void ( * new_definition_handle )( SCOREP_AnyHandle  handle,
                                      SCOREP_HandleType type );

    /**
     * Get all functions for events, attributed to their SCOREP_Substrates_EventType
     * @param mode defines which function set should be returned either for disabled or enabled recording.
     * @param functions a pointer to the functions that are assigned to the types.
     *  The returned array MUST hold SCOREP_SUBSTRATES_NUM_EVENTS elements.
     *  Not-implemented functions should be set to NULL.
     *  The array will NOT be free'd by Score-P.
     * @return MUST return SCOREP_SUBSTRATES_NUM_EVENTS (see <tt>SCOREP_SubstrateEvents.h</tt>)
     */
    uint32_t ( * get_event_functions )( SCOREP_Substrates_Mode       mode,
                                        SCOREP_Substrates_Callback** functions );

    /**
     * Provide plugins with pointers to functions that can be used to get meta data about handles.
     * @param callbacks the provided function callbacks
     * @param sizeof(SCOREP_SubstrateCallbacks) The plugin should care that its version of SCOREP_SubstrateCallbacks is smaller or equal size
     */
    void ( * set_callbacks )( const SCOREP_SubstratePluginCallbacks* callbacks,
                              size_t                                 size );

    /**
     * Provide Score-P with additional information about requirements, see SCOREP_SubstratesRequirementFlag for details.
     * If this function is not implemented, the default is assumed (0).
     * This can be called at any time by any thread depending on the flag that is queried.
     * Plugins must take care that they return 0 if flag is greater than SCOREP_SUBSTRATES_NUM_REQUIREMENT
     * Plugins must always return the same value for a given flag during one execution.
     * @param flag the requirement flag that is queried
     * @return the setting for the requirement flag, which highly depends on the type of flag
     */
    int64_t ( * get_requirement )( SCOREP_Substrates_RequirementFlag flag );

    /**
     *  for future extensions
     *  Plugins must set this to 0 (e.g., via memset)
     *
     *  When a new function is added in Score-P, SCOREP_SUBSTRATE_PLUGIN_UNDEFINED_MANAGEMENT_FUNCTIONS should be decreased by 1.
     *  Score-P should check for an appropriate plugin_version before calling the new function.
     *  Otherwise, Score-P is not able to check whether this is set to 0 but might use an invalid function.
     *  This can be avoided by enforcing a correct plugin_version.
     */
    void( *undeclared[ SCOREP_SUBSTRATE_PLUGIN_UNDEFINED_MANAGEMENT_FUNCTIONS ] )( void );
} SCOREP_SubstratePluginInfo;

#endif /* SCOREP_SUBSTRATE_PLUGINS_H */
