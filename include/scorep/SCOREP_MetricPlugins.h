/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013,
 *    RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2013,
 *    Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2013, 2015-2016,
 *    Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2013,
 *    University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2013,
 *    Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2013,
 *    German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2013,
 *    Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 *
 */

/**
 * @file
 *
 * @brief Description of the metric plugin header.
 *        For information on how to use metric plugins, please refer to @secref{metric_plugins}.
 */


#ifndef SCOREP_METRIC_PLUGINS_H
#define SCOREP_METRIC_PLUGINS_H

/**
 * The developer of a metric plugin should provide a README file which
 * explains how to compile, install and use the plugin. In particular,
 * the supported metrics should be described in the README file.
 *
 * Each metric plugin has to include <tt>SCOREP_MetricPlugins.h</tt>
 * and implement a 'get_info' function. Therefore, use the
 * #SCOREP_METRIC_PLUGIN_ENTRY macro and provide the name of the plugin
 * library as the argument.
 * For example, the metric plugin libexample_plugin.so should use
 * #SCOREP_METRIC_PLUGIN_ENTRY( example_plugin ).
 *
 * It is encouraged to use the "_plugin" suffix on the name to avoid
 * conflicts with existing libraries, e.g., libsensors_plugin.so using
 * the existing libsensors.so.
 *
 * @section mandatoryF Mandatory functions
 * See each function for details.
 *
 * @ref SCOREP_Metric_Plugin_Info::initialize "initialize"
 *
 * Check requirements and initialize the plugin.
 *
 * @ref SCOREP_Metric_Plugin_Info::get_event_info "get_event_info"
 *
 * A user specifies a SCOREP_METRIC_EXAMPLE_PLUGIN=token1,token2,...
 * This function provides information about the metric(s) corresponding
 * to this token. The total list of metrics returned for all tokens will
 * then be recorded by the plugin.
 *
 * @ref SCOREP_Metric_Plugin_Info::add_counter "add_counter"
 *
 * The function is called for and sets each of the metrics to be recorded
 * by the plugin. It provides a unique ID for each metric.
 *
 * @ref SCOREP_Metric_Plugin_Info::finalize "finalize"
 *
 * Clean up the resources used by the metric plugin.
 *
 * @section mandatoryV Mandatory variables
 *
 * @ref SCOREP_Metric_Plugin_Info::run_per "run_per"
 *
 * Defines how many threads should record the metrics of a plugin.
 *
 * @ref SCOREP_Metric_Plugin_Info::sync "sync"
 *
 * Defines synchronicity type of a metric plugin. A metric plugin can
 * <ul>
 * <li> provide a metric value for each event
 * (#SCOREP_METRIC_STRICTLY_SYNC) </li>
 * <li> optionally provide a metric value for each Score-P event
 * (#SCOREP_METRIC_SYNC) </li>
 * <li> measure metric values independently of Score-P events,
 * but collect them in Score-p during a Score-P event
 * (#SCOREP_METRIC_ASYNC_EVENT) </li>
 * <li> measure all metric values independently of events and collect them
 * once at the very end of execution (#SCOREP_METRIC_ASYNC) </li>
 * </ul>
 *
 * @ref SCOREP_Metric_Plugin_Info::plugin_version "plugin_version"
 *
 * Should be set to #SCOREP_METRIC_PLUGIN_VERSION
 *
 *
 * Depending on the plugin's synchronicity type there are some optional
 * functions and variables.
 *
 * @section optionalF Optional functions
 *
 * @ref SCOREP_Metric_Plugin_Info::get_current_value "get_current_value"
 *
 * Used by strictly synchronous metric plugins only. Returns value of
 * requested metric.
 *
 * @ref SCOREP_Metric_Plugin_Info::get_optional_value "get_optional_value"
 *
 * Used by synchronous metric plugins, but not by strictly synchronous
 * ones. This function requests current value of a metric, but it is
 * valid that no value is returned (read: no update for this metric
 * available).
 *
 * @ref SCOREP_Metric_Plugin_Info::get_all_values "get_all_values"
 *
 * Used by asynchronous metric plugins. This function is used to request
 * values of a asynchronous metric. The metric will return an arbitrary
 * number of timestamp-value-pairs.
 *
 * @ref SCOREP_Metric_Plugin_Info::set_clock_function "set_clock_function"
 *
 * Used by asynchronous metric plugins. This function passes a function
 * to the plugin, which can be used by the plugin to get a Score-P valid
 * timestamp.
 *
 * @section optionalV Optional variables
 *
 * @ref SCOREP_Metric_Plugin_Info::delta_t "delta_t"
 *
 * Defines interval between two calls to update metric value.
 * Ignored for strictly synchronous plugins.
 */


#include <stdbool.h>

#include <scorep/SCOREP_MetricTypes.h>


/** Current version of Score-P metric plugin interface */
#define SCOREP_METRIC_PLUGIN_VERSION 1

#ifdef __cplusplus
#   define EXTERN extern "C"
#else
#   define EXTERN extern
#endif

/** Macro used for implementation of the 'get_info' function */
#define SCOREP_METRIC_PLUGIN_ENTRY( _name ) \
    EXTERN SCOREP_Metric_Plugin_Info \
    SCOREP_MetricPlugin_ ## _name ## _get_info( void )


/* *********************************************************************
 * Type definitions
 **********************************************************************/

/**
 * @brief Properties describing a metric.
 * Provided by the @ref get_event_info function.
 */
typedef struct SCOREP_Metric_Plugin_MetricProperties
{
    /** Plugin name */
    char* name;
    /** Additional information about the metric */
    char* description;
    /** Metric mode: valid combination of ACCUMULATED|ABSOLUTE|RELATIVE + POINT|START|LAST|NEXT
     *  @see SCOREP_MetricMode
     * */
    SCOREP_MetricMode      mode;
    /** Value type: signed 64 bit integer, unsigned 64 bit integer, double */
    SCOREP_MetricValueType value_type;
    /** Base of metric: decimal, binary */
    SCOREP_MetricBase      base;
    /** Exponent to scale metric: e.g., 3 for kilo */
    int64_t                exponent;
    /** Unit string of recorded metric */
    char*                  unit;
} SCOREP_Metric_Plugin_MetricProperties;

/**
 * Information on that defines the plugin.
 * All values that are not explicitly defined should be set to 0
 */
typedef struct SCOREP_Metric_Plugin_Info
{
    /*
     * For all plugins
     */

    /**
     * Should be set to SCOREP_METRIC_PLUGIN_VERSION
     * (needed for back- and forward compatibility)
     */
    uint32_t plugin_version;

    /**
     * Defines how many threads should record the metrics of a plugin.
     * For the available options see @ref SCOREP_MetricPer.
     */
    SCOREP_MetricPer run_per;

    /**
     * Defines how metrics are measured over time and how they are collected
     * by @scorep. This setting influences when and which callback functions
     * are called by @scorep. For the available options see
     * @ref SCOREP_MetricSynchronicity.
     */
    SCOREP_MetricSynchronicity sync;

    /**
     * Set a specific interval for reading metric values. @scorep will request
     * metric values of a plugin, at the earliest, after #delta_t ticks after
     * it was last read.
     * NOTE: This is only a lower limit for the time between two reads
     * - there is no upper limit.
     * This setting is used by plugins of synchronicity type
     * #SCOREP_METRIC_SYNC, #SCOREP_METRIC_ASYNC_EVENT, and
     * #SCOREP_METRIC_ASYNC.
     * In combination with #SCOREP_METRIC_SYNC, it can be used for metrics that
     * update at known intervals and therefore reduce the over head of reading
     * unchanged values.
     * In combination with #SCOREP_METRIC_ASYNC_EVENT it can be used similarly.
     * This value is ignored for #SCOREP_METRIC_STRICTLY_SYNC metrics.
     */
    uint64_t delta_t;

    /**
     * This function is called once per process. It should check that
     * all requirements are met (e.g., are special libraries needed and
     * available, has the user appropriate rights to access implemented
     * metrics).
     * If all requirements are met, data structures used by the
     * plugin can be initialized within this function.
     *
     *  @return 0 if successful, error code if failure
     */
    int32_t ( * initialize )( void );

    /**
     * This functions is called once per process to clean up all resources
     * used by the metric plugin.
     */
    void ( * finalize )( void );

    /**
     * A user specifies a SCOREP_METRIC_EXAMPLE_PLUGIN=token1,token2,...
     * This function is called once per process and token. Each token can
     * result in any number of metrics (wildcards). The function shall provide
     * the properties of the metrics for this token.
     *
     * The total list of metrics returned by the calls for all tokens
     * comprises the metrics that will be recorded by the plugin.
     *
     * Note: The properties-array must contain an additional end entry
     *       with @ref SCOREP_Metric_Properties::name "name" = <tt>NULL</tt>.
     *
     * Note: The properties-array memory and all indirect pointers are managed
     *       by Score-P now. Make sure the memory remains valid and unmodified.
     *       All memory may be released with <tt>free</tt> by Score-P. Make
     *       sure that all provided pointers are created by malloc/strdup/....
     *
     *  @param token      String that describes one or multiple metrics.
     *
     *  @return properties Meta data about the metrics available for this token.
     */
    SCOREP_Metric_Plugin_MetricProperties* ( *get_event_info )( char* token );

    /**
     * Depending on #run_per, this function is called per thread, per
     * process, per host, or only on a single thread. Further it is called for
     * each metric as returned by the calls to #get_event_info.
     *
     * The function sets up the measurement for the requested metric and
     * returns a non-negative unique ID which is from now on used to refer
     * to this metric.
     *
     *  @param metric_name       Name of an individual metric
     *
     *  @return non-negative ID of requested metric
     *          or negative value in cased of failure
     */
    int32_t ( * add_counter )( char* metric_name );

    /**
     * This function shall provide the current value of a metric.
     * It must be implemented by strictly synchronous metric plugins.
     * It is called according to the #run_per specification.
     *
     *  @param id               Metric id (see #add_counter).
     *
     *  @return Current value of requested metric. For metrics of
     *          @ref SCOREP_Metric_Plugin_MetricProperties::value_type "value_type"
     *          other than UINT64, the data should be reinterpreted to a
     *          UINT64 using a union.
     *
     */
    uint64_t ( * get_current_value )( int32_t id );

    /**
     * This function provides the current value of a metric if available.
     * It must be implemented by synchronous metric plugins.
     * It is called according to the #run_per specification.
     *
     *  @param      id          Metric id (see #add_counter).
     *  @param[out] value       Current value of requested metric.
     *          For metrics of
     *          @ref SCOREP_Metric_Plugin_MetricProperties::value_type "value_type"
     *          other than UINT64, the data should be reinterpreted to a
     *          UINT64 using a union.
     *
     *  @return True if value of requested metric was written,
     *          otherwise false.
     */
    bool ( * get_optional_value )( int32_t   id,
                                   uint64_t* value );

    /**
     * When this callback is implemented, Score-P calls it once to provide
     * a clock function that allows the plugin to read the current time in
     * Score-P ticks. This should be used by asynchronous metric plugins.
     *
     * Note: This function is called before #initialize.
     *
     * @param clock_time Function pointer to Score-P's clock function.
     */
    void ( * set_clock_function )( uint64_t ( * clock_time )( void ) );

    /**
     * This function provides the recorded value of the selected metric.
     * It must be implemented by asynchronous metric plugins.
     * The timestamps in the returned list should correspond to the clock
     * provided by #set_clock_function. Further, all values (timestamps)
     * should lie within within the following interval:
     * #synchronize(#SCOREP_METRIC_SYNCHRONIZATION_MODE_BEGIN|#SCOREP_METRIC_SYNCHRONIZATION_MODE_BEGIN_MPP),
     * #synchronize(#SCOREP_METRIC_SYNCHRONIZATION_MODE_END).
     * Score-P takes ownership of the <tt>*time_value_list</tt> memory.
     * Make sure the memory remains valid and is never modified by the plugin.
     * Score-P will release the memory using <tt>free</tt>.
     * The pointer must be created directly by malloc/calloc etc. directly.
     * Do not use a memory pool / pointer with offset into a larger memory etc.
     *
     *  @param      id               Metric id (see #add_counter).
     *  @param[out] time_value_list  Pointer to list with return values
     *                               (pairs of timestamp and value).
     *
     *  @see SCOREP_MetricSynchronizationMode
     *
     *  @return Number of elements within <tt>*time_value_list</tt>
     */
    uint64_t ( * get_all_values )( int32_t                      id,
                                   SCOREP_MetricTimeValuePair** time_value_list );

    /* Since of Score-P metric plugin interface version 1 */

    /**
     * This callback is used for stating and stopping the measurement of
     * asynchronous metrics and can also be used for time synchronization
     * purposes. This function is called for all threads in the application,
     * but the threads that handle the metric plugin according to #run_per
     * will be marked as is_responsible.
     * The function will be called approximately at the same time for all
     * threads:
     * <ul>
     * <li> Once the beginning with #SCOREP_METRIC_SYNCHRONIZATION_MODE_BEGIN
     * or #SCOREP_METRIC_SYNCHRONIZATION_MODE_BEGIN_MPP for (non-)MPI
     *   programs respectively. </li>
     * <li> Once at the end with #SCOREP_METRIC_SYNCHRONIZATION_MODE_END
     * For asynchronous metrics, starting and stopping a measurement should
     * be done in this function, not in #add_counter.
     *
     * @param is_responsible   Flag to mark responsibility as per #run_per
     * @param sync_mode        Mode of synchronization point, e.g.
     *                         #SCOREP_METRIC_SYNCHRONIZATION_MODE_BEGIN,
     *                         #SCOREP_METRIC_SYNCHRONIZATION_MODE_BEGIN_MPP,
     *                         #SCOREP_METRIC_SYNCHRONIZATION_MODE_END
     *
     * @see SCOREP_MetricSynchronizationMode
     */
    void ( * synchronize )( bool                             is_responsible,
                            SCOREP_MetricSynchronizationMode sync_mode );


    /** Reserved space for future features, should be zeroed */
    uint64_t reserved[ 92 ];
} SCOREP_Metric_Plugin_Info;


#endif /* SCOREP_METRIC_PLUGINS_H */
