/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2011,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2011,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2011, 2014-2016,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2011,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2011, 2013-2014,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2011, 2014,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2011,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 *
 */

#ifndef SCOREP_TAU_H
#define SCOREP_TAU_H



/**
 * @file
 *
 * @ingroup    TAU
 *
 * @brief Implementation of the TAU adapter functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <scorep/SCOREP_PublicTypes.h>
#include <scorep/SCOREP_User.h>
#include <scorep/SCOREP_User_Types.h>
#include <scorep/SCOREP_User_Functions.h>
#include <scorep/SCOREP_PublicTypes.h>


/**
 * Initialize the measurement system from the adapter layer. This function
 * needs to be called at least once by an (arbitrary) adapter before any other
 * measurement API function is called. Calling other API functions before is
 * seen as undefined behaviour. The first call to this function triggers the
 * initialization of all adapters in use.
 *
 * For performance reasons the adapter should keep track of it's
 * initialization status and call this function only once.
 *
 * Calling this function several times does no harm to the measurement system.
 *
 * Each arising error leads to a fatal abortion of the program.
 *
 * @note The MPI adapter needs special treatment, see
 * SCOREP_InitMppMeasurement().
 *
 * @see SCOREP_FinalizeMeasurement()
 */
void
SCOREP_Tau_InitMeasurement( void );


/**
 * Type used in specifying line numbers.
 * @see SCOREP_Tau_DefineRegion()
 */
/* typedef uint32_t SCOREP_LineNo; */
/* typedef SCOREP_LineNo SCOREP_Tau_LineNo; */
/* #define SCOREP_Tau_LineNo SCOREP_LineNo */

typedef uint32_t SCOREP_Tau_LineNo;

typedef int ( * SCOREP_Tau_ExitCallback )( void );


/**
 * Symbolic constant representing an invalid or unknown line number.
 * @see SCOREP_Definitions_NewRegion()
 */
#define SCOREP_INVALID_LINE_NO                    0
#define SCOREP_TAU_INVALID_LINE_NO                SCOREP_INVALID_LINE_NO
#define SCOREP_TAU_INVALID_SOURCE_FILE            SCOREP_INVALID_SOURCE_FILE


/**
 * Types to be used in defining a region (SCOREP_Definitions_NewRegion()). In order to
 * track the origin of a region definition, the adapter needs to provide @e
 * his type.
 *
 */
typedef enum
{
    SCOREP_TAU_PARADIGM_USER,
    SCOREP_TAU_PARADIGM_COMPILER,

    /** Deprecated, don't use. */
    SCOREP_TAU_PARADIGM_MPP,

    SCOREP_TAU_PARADIGM_MPI,

    /** Deprecated, don't use. */
    SCOREP_TAU_PARADIGM_THREAD_FORK_JOIN,

    SCOREP_TAU_PARADIGM_OPENMP,

    /** Deprecated, don't use. */
    SCOREP_TAU_PARADIGM_THREAD_CREATE_WAIT,

    /** Deprecated, don't use. */
    SCOREP_TAU_PARADIGM_ACCELERATOR,

    SCOREP_TAU_PARADIGM_CUDA,
    SCOREP_TAU_PARADIGM_MEASUREMENT,
    SCOREP_TAU_PARADIGM_SHMEM,
    SCOREP_TAU_PARADIGM_PTHREAD,
    SCOREP_TAU_PARADIGM_OPENCL,

    SCOREP_TAU_INVALID_PARADIGM_TYPE /**< For internal use only. */
} SCOREP_Tau_ParadigmType;

#define SCOREP_TAU_ADAPTER_USER           SCOREP_TAU_PARADIGM_USER
#define SCOREP_TAU_ADAPTER_COMPILER       SCOREP_TAU_PARADIGM_COMPILER
#define SCOREP_TAU_ADAPTER_MPI            SCOREP_TAU_PARADIGM_MPI
#define SCOREP_TAU_ADAPTER_POMP           SCOREP_TAU_PARADIGM_OPENMP
#define SCOREP_TAU_ADAPTER_PTHREAD        SCOREP_TAU_PARADIGM_THREAD_CREATE_WAIT
#define SCOREP_TAU_ADAPTER_SHMEM          SCOREP_TAU_PARADIGM_SHMEM
#define SCOREP_TAU_INVALID_ADAPTER_TYPE   SCOREP_TAU_INVALID_PARADIGM_TYPE

typedef enum
{
    SCOREP_TAU_REGION_UNKNOWN = 0,
    SCOREP_TAU_REGION_FUNCTION,
    SCOREP_TAU_REGION_LOOP,
    SCOREP_TAU_REGION_USER,
    SCOREP_TAU_REGION_CODE,
    SCOREP_TAU_REGION_PHASE,
    SCOREP_TAU_REGION_DYNAMIC,

    SCOREP_TAU_REGION_DYNAMIC_PHASE,
    SCOREP_TAU_REGION_DYNAMIC_LOOP,
    SCOREP_TAU_REGION_DYNAMIC_FUNCTION,
    SCOREP_TAU_REGION_DYNAMIC_LOOP_PHASE,

    SCOREP_TAU_REGION_COLL_BARRIER,
    SCOREP_TAU_REGION_COLL_ONE2ALL,
    SCOREP_TAU_REGION_COLL_ALL2ONE,
    SCOREP_TAU_REGION_COLL_ALL2ALL,
    SCOREP_TAU_REGION_COLL_OTHER,
    SCOREP_TAU_REGION_POINT2POINT,

    SCOREP_TAU_REGION_PARALLEL,
    SCOREP_TAU_REGION_SECTIONS,
    SCOREP_TAU_REGION_SECTION,
    SCOREP_TAU_REGION_WORKSHARE,
    SCOREP_TAU_REGION_SINGLE,
    SCOREP_TAU_REGION_MASTER,
    SCOREP_TAU_REGION_CRITICAL,
    SCOREP_TAU_REGION_ATOMIC,
    SCOREP_TAU_REGION_BARRIER,
    SCOREP_TAU_REGION_IMPLICIT_BARRIER,
    SCOREP_TAU_REGION_FLUSH,
    SCOREP_TAU_REGION_CRITICAL_SBLOCK,
    SCOREP_TAU_REGION_SINGLE_SBLOCK,
    SCOREP_TAU_REGION_WRAPPER,
    SCOREP_TAU_REGION_TASK,
    SCOREP_TAU_REGION_TASK_WAIT,
    SCOREP_TAU_REGION_TASK_CREATE,
    SCOREP_TAU_REGION_ORDERED,
    SCOREP_TAU_REGION_ORDERED_SBLOCK,
    SCOREP_TAU_REGION_ARTIFICIAL,
    SCOREP_TAU_REGION_THREAD_CREATE,
    SCOREP_TAU_REGION_THREAD_WAIT,
    SCOREP_TAU_REGION_TASK_UNTIED,
    SCOREP_TAU_REGION_RMA,
    SCOREP_TAU_REGION_ALLOCATE,
    SCOREP_TAU_REGION_DEALLOCATE,
    SCOREP_TAU_REGION_REALLOCATE,

    SCOREP_TAU_INVALID_REGION_TYPE /**< For internal use only. */
} SCOREP_Tau_RegionType;

#define SCOREP_TAU_REGION_MPI_COLL_BARRIER SCOREP_TAU_REGION_COLL_BARRIER
#define SCOREP_TAU_REGION_MPI_COLL_ONE2ALL SCOREP_TAU_REGION_COLL_ONE2ALL
#define SCOREP_TAU_REGION_MPI_COLL_ALL2ONE SCOREP_TAU_REGION_COLL_ALL2ONE
#define SCOREP_TAU_REGION_MPI_COLL_ALL2ALL SCOREP_TAU_REGION_COLL_ALL2ALL
#define SCOREP_TAU_REGION_MPI_COLL_OTHER   SCOREP_TAU_REGION_COLL_OTHER

#define SCOREP_TAU_REGION_OMP_PARALLEL         SCOREP_TAU_REGION_PARALLEL
#define SCOREP_TAU_REGION_OMP_LOOP             SCOREP_TAU_REGION_LOOP
#define SCOREP_TAU_REGION_OMP_SECTIONS         SCOREP_TAU_REGION_SECTIONS
#define SCOREP_TAU_REGION_OMP_SECTION          SCOREP_TAU_REGION_SECTION
#define SCOREP_TAU_REGION_OMP_WORKSHARE        SCOREP_TAU_REGION_WORKSHARE
#define SCOREP_TAU_REGION_OMP_SINGLE           SCOREP_TAU_REGION_SINGLE
#define SCOREP_TAU_REGION_OMP_MASTER           SCOREP_TAU_REGION_MASTER
#define SCOREP_TAU_REGION_OMP_CRITICAL         SCOREP_TAU_REGION_CRITICAL
#define SCOREP_TAU_REGION_OMP_ATOMIC           SCOREP_TAU_REGION_ATOMIC
#define SCOREP_TAU_REGION_OMP_BARRIER          SCOREP_TAU_REGION_BARRIER
#define SCOREP_TAU_REGION_OMP_IMPLICIT_BARRIER SCOREP_TAU_REGION_IMPLICIT_BARRIER
#define SCOREP_TAU_REGION_OMP_FLUSH            SCOREP_TAU_REGION_FLUSH
#define SCOREP_TAU_REGION_OMP_CRITICAL_SBLOCK  SCOREP_TAU_REGION_CRITICAL_SBLOCK
#define SCOREP_TAU_REGION_OMP_SINGLE_SBLOCK    SCOREP_TAU_REGION_SINGLE_SBLOCK
#define SCOREP_TAU_REGION_OMP_WRAPPER          SCOREP_TAU_REGION_WRAPPER


/**
 * Associate a code region with a process unique file handle.
 *
 * @param regionName A meaningful name for the region, e.g. a function
 * name. The string will be copied.
 *
 * @param fileHandle A previously defined SCOREP_SourceFileHandle or
 * SCOREP_INVALID_SOURCE_FILE.
 *
 * @param beginLine The file line number where the region starts or
 * SCOREP_INVALID_LINE_NO.
 *
 * @param endLine The file line number where the region ends or
 * SCOREP_INVALID_LINE_NO.
 *
 * @param adapter The type of adapter (SCOREP_ParadigmType) that is calling.
 *
 * @param regionType The type of the region. Until now, the @a regionType was
 * not used during the measurement but during analysis. This @e may change in
 * future with e.g. dynamic regions or parameter based profiling. In the first
 * run, we can implement at least dynamic regions in the adapter.
 *
 * @note The name of e.g. Java classes, previously provided as a string to the
 * region description, should now be encoded in the region name. The region
 * description field is replaced by the adapter type as that was it's primary
 * use.
 *
 * @note During unification, we compare @a regionName, @a fileHandle, @a
 * beginLine, @a endLine and @a adapter of regions from different
 * processes. If all values are equal, we consider the regions to be equal. We
 * don't check for uniqueness of this tuple in this function, i.e. during
 * measurement, this is the adapters responsibility, but we require that every
 * call defines a unique and distinguishable region.
 *
 * @return A process unique region handle to be used in calls to
 * SCOREP_EnterRegion() and SCOREP_ExitRegion().
 *
 */

#define SCOREP_Tau_RegionHandle           uint64_t
#define SCOREP_Tau_SourceFileHandle       SCOREP_SourceFileHandle

SCOREP_Tau_RegionHandle
SCOREP_Tau_DefineRegion( const char*                 regionName,
                         SCOREP_Tau_SourceFileHandle fileHandle,
                         SCOREP_Tau_LineNo           beginLine,
                         SCOREP_Tau_LineNo           endLine,
                         SCOREP_Tau_ParadigmType     paradigm,
                         SCOREP_Tau_RegionType       regionType );

/**
 * Generate a region enter event in the measurement system.
 *
 * @param regionHandle The corresponding region for the enter event.
 */
void
SCOREP_Tau_EnterRegion( SCOREP_Tau_RegionHandle regionHandle );

/**
 * Generate a region exit event in the measurement system.
 *
 * @param regionHandle The corresponding region for the enter event.
 */
void
SCOREP_Tau_ExitRegion( SCOREP_Tau_RegionHandle regionHandle );

/**
 * Register a function that can close the callstack. This is invoked by
 * the Score-P routine that is called by atexit. Before flushing the data to
 * disk, all the open timers are closed by invoking the function callback.
 *
 * @param: callback, a function pointer. It points to the routine
 * Tau_profile_exit_all_threads.
 */
void
    SCOREP_Tau_RegisterExitCallback( SCOREP_Tau_ExitCallback );

#define SCOREP_Tau_MetricHandle            SCOREP_SamplingSetHandle
#define SCOREP_TAU_INIT_METRIC_HANDLE   SCOREP_INVALID_SAMPLING_SET

/**
 * Sets an invalid handle value.
 * @param metricHandle  The handle that is invalidated.
 */
void
SCOREP_Tau_Metric( SCOREP_Tau_MetricHandle* metricHandle );

/**
 * Defines a metric handle. Every metric must be initialized before first usage.
 * @param metricHandle Handle of the metric to be initialized.
 * @param name         Metric name.
 * @param unit         A string containing the metric unit for display.
 */
void
SCOREP_Tau_InitMetric( SCOREP_Tau_MetricHandle* metricHandle,
                       const char*              name,
                       const char*              unit );

/**
 * Adds a metric sample.
 * @param metricHandle Handle for the metric definition for which the value is given.
 * @param value        The value of the current metric sample.
 */
void
SCOREP_Tau_TriggerMetricDouble( SCOREP_Tau_MetricHandle metricHandle,
                                double                  value );

#define SCOREP_Tau_ParamHandle           SCOREP_User_ParameterHandle
#define SCOREP_TAU_INIT_PARAM_HANDLE           SCOREP_USER_INVALID_PARAMETER

/**
 * Creates a parameter event for 64-bit integer values. If the handle is not
 * initialized, it automatically initializes the handle.
 * @param paramHandle Handle for the parameter definition.
 * @param name        Name of the parameter.
 * @param value       The parameter value.
 */
void
SCOREP_Tau_Parameter_INT64( SCOREP_Tau_ParamHandle* paramHandle,
                            const char*             name,
                            int64_t                 value );

/**
 * Adds a property to the calling location. A property is a key/value pair.
 * Every key can only exist once per location. If a key already exists on a
 * location, the current value is discarded.
 * @param name  The name of the key.
 * @param value The value.
 */
void
SCOREP_Tau_AddLocationProperty( const char* name,
                                const char* value );


#ifdef __cplusplus
} /* extern C */
#endif

#endif /* SCOREP_TAU_H */
