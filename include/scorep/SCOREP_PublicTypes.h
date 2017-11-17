/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2011,
 *    RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2011,
 *    Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2011,
 *    Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2011,
 *    University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2011,
 *    Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2011,
 *    German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2011,
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
 * @brief Defines public definitions that are used internally and externally (e.g., by metric plugins, user functions, substrate plugins)
 */

#ifndef SCOREP_PUBLICTYPES_H
#define SCOREP_PUBLICTYPES_H

#include <stdint.h>

/** \defgroup Public type definitions and enums used in Score-P
 */
/*@{*/

/**
 * Type used in specifying line numbers.
 * @see SCOREP_Definitions_NewRegion()
 */
typedef uint32_t SCOREP_LineNo;



/**
 * Symbolic constant representing an invalid or unknown line number.
 * @see SCOREP_Definitions_NewRegion()
 */
#define SCOREP_INVALID_LINE_NO 0

/**
 * Opaque handle to memory that can be easily moved between
 * processes. Used for definitions as they have to be moved during
 * unification.
 */
typedef uint32_t SCOREP_Allocator_MovableMemory;

/**
 * Symbolic constant representing an invalid or NULL handle of type
 * SCOREP_Allocator_MovableMemory.
 *
 */
#define SCOREP_MOVABLE_NULL 0

/**
 * Type of a opaque handle to any definition.
 */
typedef SCOREP_Allocator_MovableMemory SCOREP_AnyHandle;

/**
 * Type of a opaque handle to a source file definition.
 * @see SCOREP_Definitions_NewSourceFile()
 */
typedef SCOREP_AnyHandle SCOREP_SourceFileHandle;

/**
 * Symbolic constant representing an invalid or unknown source file definition.
 *
 */
#define SCOREP_INVALID_SOURCE_FILE SCOREP_MOVABLE_NULL

/**
 * Type of a opaque handle to a metric definition.
 * @see SCOREP_Definitions_NewMetric()
 */
typedef SCOREP_AnyHandle SCOREP_MetricHandle;

/**
 * Symbolic constant representing an invalid or unknown metric
 * definition.
 */
#define SCOREP_INVALID_METRIC SCOREP_MOVABLE_NULL


/**
 * Type of a opaque handle to a sampling set definition.
 * @see SCOREP_Definitions_NewSamplingSet()
 */
typedef SCOREP_AnyHandle SCOREP_SamplingSetHandle;

/**
 * Symbolic constant representing an invalid or unknown metric class definition.
 */
#define SCOREP_INVALID_SAMPLING_SET SCOREP_MOVABLE_NULL


/**
 * Type of a opaque handle to a region definition.
 * @see SCOREP_Definitions_NewRegion()
 */
typedef SCOREP_AnyHandle SCOREP_RegionHandle;

/**
 * Symbolic constant representing an invalid or unknown region definition.
 */
#define SCOREP_INVALID_REGION SCOREP_MOVABLE_NULL


/**
 * Type of a opaque handle to a paradigm definition.
 * @see SCOREP_Definitions_NewParadigm()
 */
typedef SCOREP_AnyHandle SCOREP_ParadigmHandle;

/**
 * Symbolic constant representing an invalid or unknown paradigm definition.
 */
#define SCOREP_INVALID_PARADIGM SCOREP_MOVABLE_NULL



/**
 * Task Handle
 */
typedef struct SCOREP_Task* SCOREP_TaskHandle;

/**
 * \enum SCOREP_CollectiveType
 * \brief Types to specify the used collectives in calls to @a SCOREP_MpiCollectiveBegin and @a SCOREP_RmaCollectiveBegin
 */
typedef enum SCOREP_CollectiveType
{
    SCOREP_COLLECTIVE_BARRIER,                      /**< The collective is a barrier, e.g., MPI_Barrier(...), shmem_barrier(...), or shmem_barrier_all(...) */
    SCOREP_COLLECTIVE_BROADCAST,                    /**< The collective is a barrier, e.g., MPI_Bcast(...), or shmem_broadcast32(...) */
    SCOREP_COLLECTIVE_GATHER,                       /**< The collective is a simple gather operation, e.g., MPI_Gather(...) */
    SCOREP_COLLECTIVE_GATHERV,                      /**< The collective is a complex gather operation, e.g., MPI_Gatherv(...) */
    SCOREP_COLLECTIVE_SCATTER,                      /**< The collective is a simple scatter operation, e.g., MPI_Scatter(...) */
    SCOREP_COLLECTIVE_SCATTERV,                     /**< The collective is a complex scatter operation, e.g., MPI_Scatterv(...) */
    SCOREP_COLLECTIVE_ALLGATHER,                    /**< The collective is a simple allgather operation, e.g., MPI_Allgather(...), or shmem_collect64(...) */
    SCOREP_COLLECTIVE_ALLGATHERV,                   /**< The collective is a complex allgather operation, e.g., MPI_Allgatherv(...) */
    SCOREP_COLLECTIVE_ALLTOALL,                     /**< The collective is a simple all-to-all communication, e.g., MPI_Alltoall(...) */
    SCOREP_COLLECTIVE_ALLTOALLV,                    /**< The collective is a all-to-all communication with more options for sizes and displacements, e.g., MPI_Alltoallv(...) */
    SCOREP_COLLECTIVE_ALLTOALLW,                    /**< The collective is a generalized all-to-all communication, e.g., MPI_Alltoallw(...) */
    SCOREP_COLLECTIVE_ALLREDUCE,                    /**< The collective is an allreduce operation, e.g., MPI_Allreduce(...) */
    SCOREP_COLLECTIVE_REDUCE,                       /**< The collective is a reduce operation, e.g., MPI_Reduce(...), or shmem_longlong_max_to_all(...) */
    SCOREP_COLLECTIVE_REDUCE_SCATTER,               /**< The collective is a reduce-scatter operation, which combines some values and scatters the results, e.g., MPI_Reduce_scatter(...) */
    SCOREP_COLLECTIVE_REDUCE_SCATTER_BLOCK,         /**< The collective is a reduce scatter block operation, e.g., MPI_Reduce_scatter_block(...) */
    SCOREP_COLLECTIVE_SCAN,                         /**< The collective is a scan operation, where partial reductions of data is computed, e.g., MPI_Scan(...) */
    SCOREP_COLLECTIVE_EXSCAN,                       /**< The collective is an exclusive scan operation, e.g., MPI_Exscan(...) */
    SCOREP_COLLECTIVE_CREATE_HANDLE,                /**< This is used by the tracing substrate to work together with OTF2  */
    SCOREP_COLLECTIVE_DESTROY_HANDLE,               /**< This is used by the tracing substrate to work together with OTF2 */
    SCOREP_COLLECTIVE_ALLOCATE,                     /**< This is used by the tracing substrate to work together with OTF2 */
    SCOREP_COLLECTIVE_DEALLOCATE,                   /**< This is used by the tracing substrate to work together with OTF2 */
    SCOREP_COLLECTIVE_CREATE_HANDLE_AND_ALLOCATE,   /**< This is used by the tracing substrate to work together with OTF2 */
    SCOREP_COLLECTIVE_DESTROY_HANDLE_AND_DEALLOCATE /**< This is used by the tracing substrate to work together with OTF2 */
} SCOREP_CollectiveType;



/**
 * Types to be used in defining a location (SCOREP_Definitions_NewLocation()).
 *
 */
#define SCOREP_LOCATION_TYPES \
    SCOREP_LOCATION_TYPE( CPU_THREAD, "CPU thread" ) \
    SCOREP_LOCATION_TYPE( GPU,        "GPU" ) \
    SCOREP_LOCATION_TYPE( METRIC,     "metric location" ) \


typedef enum SCOREP_LocationType
{
    #define SCOREP_LOCATION_TYPE( NAME, name_string ) SCOREP_LOCATION_TYPE_ ## NAME,
    SCOREP_LOCATION_TYPES
    #undef SCOREP_LOCATION_TYPE

    SCOREP_NUMBER_OF_LOCATION_TYPES,
    SCOREP_INVALID_LOCATION_TYPE /**< For internal use only. */
} SCOREP_LocationType;

/**
 * General Lock Type.
 */
typedef enum SCOREP_LockType
{
    /** Exclusive lock. No other lock will be granted.
     */
    SCOREP_LOCK_EXCLUSIVE,
    /** Shared lock. Other shared locks will be granted, but no exclusive
     *  locks.
     */
    SCOREP_LOCK_SHARED,

    SCOREP_INVALID_LOCK_TYPE /**< For internal use only. */
} SCOREP_LockType;

/**
 * Type of MPI Ranks. Type of MPI ranks always int.
 */
typedef int SCOREP_MpiRank;


/**
 * Type of a MPI Non-blocking communication request id.
 */
typedef uint64_t SCOREP_MpiRequestId;


/**
 * Symbolic constant representing an invalid or unknown rank.
 * @see SCOREP_MpiCollective()
 */
#define SCOREP_INVALID_ROOT_RANK -1


/**
 * Types to be used in defining a region (SCOREP_Definitions_NewRegion()). In order to
 * track the origin of a region definition, the adapter needs to provide @e
 * his type.
 *
 */

/**
 * \enum SCOREP_ParadigmClass
 * \brief defines classes of paradigms that are monitored
 * Types:
 * - SCOREP_PARADIGM_CLASS_MPP refers to any multi processing based paradigms (e.g., MPI, SHMEM)
 * - SCOREP_PARADIGM_CLASS_THREAD_FORK_JOIN refers to any thread parallel fork-join based paradigms (e.g., OpenMP)
 * - SCOREP_PARADIGM_CLASS_THREAD_CREATE_WAIT refers to any thread parallel create wait based paradigms (e.g., PThreads)
 * - SCOREP_PARADIGM_CLASS_ACCELERATOR refers to any accelerator based paradigms
 * - SCOREP_INVALID_PARADIGM_CLASS for internal use only
 */
#define SCOREP_PARADIGM_CLASSES \
    SCOREP_PARADIGM_CLASS( MPP,                "multi-process", PROCESS ) \
    SCOREP_PARADIGM_CLASS( THREAD_FORK_JOIN,   "fork/join",     THREAD_FORK_JOIN ) \
    SCOREP_PARADIGM_CLASS( THREAD_CREATE_WAIT, "create/wait",   THREAD_CREATE_WAIT ) \
    SCOREP_PARADIGM_CLASS( ACCELERATOR,        "accelerator",   ACCELERATOR )


typedef enum SCOREP_ParadigmClass
{
#define SCOREP_PARADIGM_CLASS( NAME, name, OTF2_NAME ) \
    SCOREP_PARADIGM_CLASS_ ## NAME,
    SCOREP_PARADIGM_CLASSES

#undef SCOREP_PARADIGM_CLASS

    SCOREP_INVALID_PARADIGM_CLASS /**< For internal use only. */
} SCOREP_ParadigmClass;


/**
 * \enum SCOREP_ParadigmType
 * \brief defines paradigms that are be monitored
 *
 * ! Keep MPI first after the non-parallel paradigms
 *
 * Types:
 * - SCOREP_PARADIGM_MEASUREMENT refers to Score-P internals
 * - SCOREP_PARADIGM_USER refers to user instrumentation
 * - SCOREP_PARADIGM_COMPILER refers to compiler instrumentation
 * - SCOREP_PARADIGM_SAMPLING refers to sampling
 * - SCOREP_PARADIGM_MEMORY refers to a memory region (malloc/realloc/...)
 * - SCOREP_PARADIGM_MPI refers to MPI instrumentation
 * - SCOREP_PARADIGM_SHMEM refers to MPI instrumentation
 * - SCOREP_PARADIGM_OPENMP refers to OpenMP instrumentation
 * - SCOREP_PARADIGM_PTHREAD refers to Pthread instrumentation
 * - SCOREP_PARADIGM_CUDA refers to CUDA instrumentation
 * - SCOREP_PARADIGM_OPENCL refers to OpenCL instrumentation
 * - SCOREP_PARADIGM_OPENACC refers to OpenACC instrumentation
 * - SCOREP_INVALID_PARADIGM_TYPE for internal use only
 */
#define SCOREP_PARADIGMS \
    SCOREP_PARADIGM( MEASUREMENT,        "measurement",        MEASUREMENT_SYSTEM ) \
    SCOREP_PARADIGM( USER,               "user",               USER ) \
    SCOREP_PARADIGM( COMPILER,           "compiler",           COMPILER ) \
    SCOREP_PARADIGM( SAMPLING,           "sampling",           SAMPLING ) \
    SCOREP_PARADIGM( MEMORY,             "memory",             NONE ) \
    SCOREP_PARADIGM( MPI,                "mpi",                MPI ) \
    SCOREP_PARADIGM( SHMEM,              "shmem",              SHMEM ) \
    SCOREP_PARADIGM( OPENMP,             "openmp",             OPENMP ) \
    SCOREP_PARADIGM( PTHREAD,            "pthread",            PTHREAD ) \
    SCOREP_PARADIGM( CUDA,               "cuda",               CUDA ) \
    SCOREP_PARADIGM( OPENCL,             "opencl",             OPENCL ) \
    SCOREP_PARADIGM( OPENACC,            "openacc",            OPENACC )


typedef enum SCOREP_ParadigmType
{
#define SCOREP_PARADIGM( NAME, name_str, OTF2_NAME ) \
    SCOREP_PARADIGM_ ## NAME,
    SCOREP_PARADIGMS

#undef SCOREP_PARADIGM

    SCOREP_INVALID_PARADIGM_TYPE /**< For internal use only. */
} SCOREP_ParadigmType;

/**
 * \enum SCOREP_ParameterType
 * \brief defines types to be used in defining a parameter for parameter based profiling
 * (SCOREP_Definitions_NewParameter()).
 *
 */
typedef enum SCOREP_ParameterType
{
    SCOREP_PARAMETER_INT64,       /**< The parameter is an int64_t */
    SCOREP_PARAMETER_UINT64,      /**< The parameter is an uint64_t */
    SCOREP_PARAMETER_STRING,      /**< The parameter is a string */

    SCOREP_INVALID_PARAMETER_TYPE /**< For internal use only. */
} SCOREP_ParameterType;


/**
 * \enum SCOREP_RegionType
 * \brief specifies a Region
 *
 * Types to be used in defining a region (SCOREP_Definitions_NewRegion()). These types
 * are currently not used inside the measurement system. This @e may change in
 * future if we are going to implement phases/dynamic regions etc. inside the
 * measurement system as opposed to inside the adapters or as a postprocessing
 * step. The names should be self explanatory; most of them are already used
 * (whith a different prefix) in VampiTrace and Scalasca.
 *
 * Types:
 * - SCOREP_REGION_UNKNOWN The type of the region is unknown / not defined
 * - SCOREP_REGION_FUNCTION The region is defined by compiler instrumentation/sampling and defines a code function
 * - SCOREP_REGION_LOOP The region represents a loop in the source code (used by Opari)
 * - SCOREP_REGION_USER The region is a user region, e.g., an Opari user region
 * - SCOREP_REGION_CODE The region represents a code region
 * - SCOREP_REGION_PHASE (Currently not used)
 * - SCOREP_REGION_DYNAMIC (Currently not used)
 * - SCOREP_REGION_DYNAMIC_PHASE (Currently not used)
 * - SCOREP_REGION_DYNAMIC_LOOP (Currently not used)
 * - SCOREP_REGION_DYNAMIC_FUNCTION (Currently not used)
 * - SCOREP_REGION_DYNAMIC_LOOP_PHASE (Currently not used)
 * - SCOREP_REGION_COLL_ONE2ALL Represents a collective communication region with one2all communication
 * - SCOREP_REGION_COLL_ALL2ONE Represents a collective communication region with all2one communication
 * - SCOREP_REGION_COLL_ALL2ALL Represents a collective communication region with all2all communication
 * - SCOREP_REGION_COLL_OTHER Represents a collective communication region that is neither one2all, nor all2one, nor all2all
 * - SCOREP_REGION_POINT2POINT Represents a point2point communication region
 * - SCOREP_REGION_PARALLEL Represents an (OpenMP) parallel region
 * - SCOREP_REGION_SECTIONS Represents an (OpenMP) sections region
 * - SCOREP_REGION_SECTION Represents an (OpenMP) section region
 * - SCOREP_REGION_WORKSHARE Represents an (OpenMP) workshare region
 * - SCOREP_REGION_SINGLE Represents an (OpenMP) single region
 * - SCOREP_REGION_MASTER Represents an (OpenMP) master region
 * - SCOREP_REGION_CRITICAL Represents an (OpenMP) critical region
 * - SCOREP_REGION_ATOMIC Represents an atomic region
 * - SCOREP_REGION_BARRIER Represents a barrier
 * - SCOREP_REGION_IMPLICIT_BARRIER Represents an implicit barrier (that is implicitely given but not explicitely defined)
 * - SCOREP_REGION_FLUSH Represents an (OpenMP) flush region
 * - SCOREP_REGION_CRITICAL_SBLOCK Represents an sblock within a (OpenMP) critical region
 * - SCOREP_REGION_SINGLE_SBLOCK Represents an sblock within a (OpenMP) single region
 * - SCOREP_REGION_WRAPPER Represents a wrapper region (e.g., from interpositioning)
 * - SCOREP_REGION_TASK Represents a (OpenMP) task region, within SCOREP_REGION_TASK_CREATE
 * - SCOREP_REGION_TASK_UNTIED Represents a (OpenMP) untied task region
 * - SCOREP_REGION_TASK_WAIT Represents a (OpenMP) taskwait region
 * - SCOREP_REGION_TASK_CREATE Represents a created (OpenMP) task region
 * - SCOREP_REGION_ORDERED Represents an (OpenMP) ordered region
 * - SCOREP_REGION_ORDERED_SBLOCK Represents an sblock within a (OpenMP) ordered region
 * - SCOREP_REGION_ARTIFICIAL Represents an artificial region
 * - SCOREP_REGION_RMA Represents an RMA region
 * - SCOREP_REGION_THREAD_CREATE Represents the creation of a thread
 * - SCOREP_REGION_THREAD_WAIT Represents the creation of a thread
 * - SCOREP_REGION_ALLOCATE Represents a region where memory is allocated, e.g., MPI_Alloc_mem
 * - SCOREP_REGION_DEALLOCATE Represents a region where memory is deallocated
 * - SCOREP_REGION_REALLOCATE Represents a region where memory is reallocated
 */
#define SCOREP_REGION_TYPES \
    SCOREP_REGION_TYPE( COLL_ONE2ALL, "one2all" ) \
    SCOREP_REGION_TYPE( COLL_ALL2ONE, "all2one" ) \
    SCOREP_REGION_TYPE( COLL_ALL2ALL, "all2all" ) \
    SCOREP_REGION_TYPE( COLL_OTHER,   "other collective" ) \
    SCOREP_REGION_TYPE( POINT2POINT,  "point2point" ) \
    SCOREP_REGION_TYPE( PARALLEL,     "parallel" ) \
    SCOREP_REGION_TYPE( SECTIONS,     "sections" ) \
    SCOREP_REGION_TYPE( SECTION,      "section" ) \
    SCOREP_REGION_TYPE( WORKSHARE,    "workshare" ) \
    SCOREP_REGION_TYPE( SINGLE,       "single" ) \
    SCOREP_REGION_TYPE( MASTER,       "master" ) \
    SCOREP_REGION_TYPE( CRITICAL,     "critical" ) \
    SCOREP_REGION_TYPE( ATOMIC,       "atomic" ) \
    SCOREP_REGION_TYPE( BARRIER,      "barrier" ) \
    SCOREP_REGION_TYPE( IMPLICIT_BARRIER, "implicit barrier" ) \
    SCOREP_REGION_TYPE( FLUSH,        "flush" ) \
    SCOREP_REGION_TYPE( CRITICAL_SBLOCK, "critical sblock" ) \
    SCOREP_REGION_TYPE( SINGLE_SBLOCK, "single sblock" ) \
    SCOREP_REGION_TYPE( WRAPPER,      "wrapper" ) \
    SCOREP_REGION_TYPE( TASK,         "task" ) \
    SCOREP_REGION_TYPE( TASK_UNTIED,  "untied task" ) \
    SCOREP_REGION_TYPE( TASK_WAIT,    "taskwait" ) \
    SCOREP_REGION_TYPE( TASK_CREATE,  "task create" ) \
    SCOREP_REGION_TYPE( ORDERED,      "ordered" ) \
    SCOREP_REGION_TYPE( ORDERED_SBLOCK, "ordered sblock" ) \
    SCOREP_REGION_TYPE( ARTIFICIAL,   "artificial" ) \
    SCOREP_REGION_TYPE( RMA,          "rma" ) \
    SCOREP_REGION_TYPE( THREAD_CREATE, "thread create" ) \
    SCOREP_REGION_TYPE( THREAD_WAIT,  "thread wait" ) \
    SCOREP_REGION_TYPE( ALLOCATE,     "allocate" ) \
    SCOREP_REGION_TYPE( DEALLOCATE,   "deallocate" ) \
    SCOREP_REGION_TYPE( REALLOCATE,   "reallocate" )


#define SCOREP_REGION_TYPE( NAME, name_str ) \
    SCOREP_REGION_ ## NAME,

typedef enum SCOREP_RegionType
{
    SCOREP_REGION_UNKNOWN = 0,
    SCOREP_REGION_FUNCTION,
    SCOREP_REGION_LOOP,
    SCOREP_REGION_USER,
    SCOREP_REGION_CODE,

    SCOREP_REGION_PHASE,
    SCOREP_REGION_DYNAMIC,
    SCOREP_REGION_DYNAMIC_PHASE,
    SCOREP_REGION_DYNAMIC_LOOP,
    SCOREP_REGION_DYNAMIC_FUNCTION,
    SCOREP_REGION_DYNAMIC_LOOP_PHASE,

    SCOREP_REGION_TYPES

    SCOREP_INVALID_REGION_TYPE /**< For internal use only. */
} SCOREP_RegionType;

#undef SCOREP_REGION_TYPE



/**
 * \enum SCOREP_RmaSyncType
 * \brief Type of direct RMA synchronization call
 *
 * Types:
 * - SCOREP_RMA_SYNC_TYPE_MEMORY Synchronize memory copy.
 * - SCOREP_RMA_SYNC_TYPE_NOTIFY_IN  Incoming remote notification.
 * - SCOREP_RMA_SYNC_TYPE_NOTIFY_OUT Outgoing remote notification
 * - SCOREP_INVALID_RMA_SYNC_TYPE for internal use only
 */

#define SCOREP_RMA_SYNC_TYPES \
    SCOREP_RMA_SYNC_TYPE( MEMORY, memory, "memory" )             \
    SCOREP_RMA_SYNC_TYPE( NOTIFY_IN, notify_in, "notify in" )    \
    SCOREP_RMA_SYNC_TYPE( NOTIFY_OUT, notify_out, "notify out" )

typedef enum SCOREP_RmaSyncType
{
#define SCOREP_RMA_SYNC_TYPE( upper, lower, name )  SCOREP_RMA_SYNC_TYPE_ ## upper,
    SCOREP_RMA_SYNC_TYPES
    #undef SCOREP_RMA_SYNC_TYPE

    SCOREP_INVALID_RMA_SYNC_TYPE /**< For internal use only. */
} SCOREP_RmaSyncType;



/**
 * \enum SCOREP_RmaSyncLevel
 * \brief specifies a RMA synchronization level, used by RMA records to be passed to SCOREP_Rma*() functions.
 *
 * Types:
 * - SCOREP_RMA_SYNC_LEVEL_NONE No process synchronization or access completion (e.g., * MPI_Win_post)
 * - SCOREP_RMA_SYNC_LEVEL_PROCESS Synchronize processes (e.g., MPI_Win_create/free)
 * - SCOREP_RMA_SYNC_LEVEL_MEMORY Complete memory accesses (e.g., MPI_Win_complete, MPI_Win_wait)
 * - SCOREP_RMA_SYNC_LEVELS for internal use only
 */

#define SCOREP_RMA_SYNC_LEVELS \
    SCOREP_RMA_SYNC_LEVEL( NONE, none, "none", 0 ) \
    SCOREP_RMA_SYNC_LEVEL( PROCESS, process, "process", 1 << 0 ) \
    SCOREP_RMA_SYNC_LEVEL( MEMORY, memory, "memory", 1 << 1 )

typedef enum SCOREP_RmaSyncLevel
{
#define SCOREP_RMA_SYNC_LEVEL( upper, lower, name, value ) \
    SCOREP_RMA_SYNC_LEVEL_ ## upper = value,

    SCOREP_RMA_SYNC_LEVELS
#undef SCOREP_RMA_SYNC_LEVEL
} SCOREP_RmaSyncLevel;


/**
 * \enum SCOREP_RmaAtomicType
 * \brief specifies a RMA Atomic Operation Type.
 *
 * Types:
 * - SCOREP_RMA_ATOMIC_TYPE_ACCUMULATE accumulate
 * - SCOREP_RMA_ATOMIC_TYPE_INCREMENT increment
 * - SCOREP_RMA_ATOMIC_TYPE_TEST_AND_SET test and set
 * - SCOREP_RMA_ATOMIC_TYPE_COMPARE_AND_SWAP compare and swap
 * - SCOREP_RMA_ATOMIC_TYPE_SWAP swap
 * - SCOREP_RMA_ATOMIC_TYPE_FETCH_AND_ADD fetch and add
 * - SCOREP_RMA_ATOMIC_TYPE_FETCH_AND_INCREMENT fetch and increment
 * - SCOREP_RMA_ATOMIC_TYPE_ADD add
 * - SCOREP_INVALID_RMA_ATOMIC_TYPE for internal use only
 */


#define SCOREP_RMA_ATOMIC_TYPES \
    SCOREP_RMA_ATOMIC_TYPE( ACCUMULATE, accumulate,      "accumulate" )   \
    SCOREP_RMA_ATOMIC_TYPE( INCREMENT, increment,       "increment" )     \
    SCOREP_RMA_ATOMIC_TYPE( TEST_AND_SET, test_and_set,    "test and set" ) \
    SCOREP_RMA_ATOMIC_TYPE( COMPARE_AND_SWAP, compare_and_swap, "compare and swap" ) \
    SCOREP_RMA_ATOMIC_TYPE( SWAP, swap, "swap" ) \
    SCOREP_RMA_ATOMIC_TYPE( FETCH_AND_ADD, fetch_and_add, "fetch and add" ) \
    SCOREP_RMA_ATOMIC_TYPE( FETCH_AND_INCREMENT, fetch_and_increment, "fetch and increment" ) \
    SCOREP_RMA_ATOMIC_TYPE( ADD, add, "add" )

typedef enum SCOREP_RmaAtomicType
{
#define SCOREP_RMA_ATOMIC_TYPE( upper, lower, name ) SCOREP_RMA_ATOMIC_TYPE_ ## upper,
    SCOREP_RMA_ATOMIC_TYPES
    #undef SCOREP_RMA_ATOMIC_TYPE

    SCOREP_INVALID_RMA_ATOMIC_TYPE
} SCOREP_RmaAtomicType;


/**
 * \enum SCOREP_SamplingSetClass
 * \brief Class of locations which recorded a sampling set.
 */
typedef enum SCOREP_SamplingSetClass
{
    SCOREP_SAMPLING_SET_ABSTRACT, /**< The sampling set is more complicated, e.g., refers to a number of locations. */
    SCOREP_SAMPLING_SET_CPU,      /**< The sampling set refers to a CPU. */
    SCOREP_SAMPLING_SET_GPU       /**< The sampling set refers to a GPU. */
} SCOREP_SamplingSetClass;

/**
 * \enum SCOREP_MetricScope
 * \brief Types to be used in defining the scope of a scoped sampling set.
 *
 */
typedef enum SCOREP_MetricScope
{
    /** Scope of a metric is another location. */
    SCOREP_METRIC_SCOPE_LOCATION         = 0,
    /** Scope of a metric is a location group. */
    SCOREP_METRIC_SCOPE_LOCATION_GROUP   = 1,
    /** Scope of a metric is a system tree node. */
    SCOREP_METRIC_SCOPE_SYSTEM_TREE_NODE = 2,
    /** Scope of a metric is a generic group of locations. */
    SCOREP_METRIC_SCOPE_GROUP            = 3,

    SCOREP_INVALID_METRIC_SCOPE /**< For internal use only. */
} SCOREP_MetricScope;

/**
 * \enum SCOREP_MetricOccurrence
 * \brief Types to be used in defining the occurrence of a sampling set.
 *
 */
typedef enum SCOREP_MetricOccurrence
{
    /** Metric occurs at every region enter and leave. */
    SCOREP_METRIC_OCCURRENCE_SYNCHRONOUS_STRICT = 0,
    /** Metric occurs only at a region enter and leave, but does not need to
     *  occur at every enter/leave. */
    SCOREP_METRIC_OCCURRENCE_SYNCHRONOUS        = 1,
    /** Metric can occur at any place i.e. it is not related to region enter and
     *  leaves. */
    SCOREP_METRIC_OCCURRENCE_ASYNCHRONOUS       = 2,

    SCOREP_INVALID_METRIC_OCCURRENCE /**< For internal use only. */
} SCOREP_MetricOccurrence;


/**
 * \enum SCOREP_Ipc_Datatype
 * \brief specifies an inter process communication data types
 *
 * Types:
 * - SCOREP_IPC_BYTE byte
 * - SCOREP_IPC_CHAR char
 * - SCOREP_IPC_UNSIGNED_CHAR unsigned char
 * - SCOREP_IPC_INT  int
 * - SCOREP_IPC_UNSIGNED unsigned int
 * - SCOREP_IPC_INT32_T int32_t
 * - SCOREP_IPC_UINT32_T uint32_t
 * - SCOREP_IPC_INT64_T int64_t
 * - SCOREP_IPC_UINT64_T uint64_t
 * - SCOREP_IPC_DOUBLE double
 */

#define SCOREP_IPC_DATATYPES \
    SCOREP_IPC_DATATYPE( BYTE ) \
    SCOREP_IPC_DATATYPE( CHAR ) \
    SCOREP_IPC_DATATYPE( UNSIGNED_CHAR ) \
    SCOREP_IPC_DATATYPE( INT ) \
    SCOREP_IPC_DATATYPE( UNSIGNED ) \
    SCOREP_IPC_DATATYPE( INT32_T ) \
    SCOREP_IPC_DATATYPE( UINT32_T ) \
    SCOREP_IPC_DATATYPE( INT64_T ) \
    SCOREP_IPC_DATATYPE( UINT64_T ) \
    SCOREP_IPC_DATATYPE( DOUBLE )

typedef enum SCOREP_Ipc_Datatype
{
#define SCOREP_IPC_DATATYPE( datatype ) \
    SCOREP_IPC_ ## datatype,
    SCOREP_IPC_DATATYPES
#undef SCOREP_IPC_DATATYPE
    SCOREP_IPC_NUMBER_OF_DATATYPES
} SCOREP_Ipc_Datatype;

/**
 * \enum SCOREP_Ipc_Operation
 * \brief specifies an inter process communication operation for reduce function
 *
 * Types:
 * - SCOREP_IPC_BAND binary and
 * - SCOREP_IPC_BOR binary or
 * - SCOREP_IPC_MIN minimum
 * - SCOREP_IPC_MAX maximum
 * - SCOREP_IPC_SUM sum
 */

#define SCOREP_IPC_OPERATIONS \
    SCOREP_IPC_OPERATION( BAND ) \
    SCOREP_IPC_OPERATION( BOR ) \
    SCOREP_IPC_OPERATION( MIN ) \
    SCOREP_IPC_OPERATION( MAX ) \
    SCOREP_IPC_OPERATION( SUM )

typedef enum SCOREP_Ipc_Operation
{
#define SCOREP_IPC_OPERATION( op ) \
    SCOREP_IPC_ ## op,
    SCOREP_IPC_OPERATIONS
#undef SCOREP_IPC_OPERATION
    SCOREP_IPC_NUMBER_OF_OPERATIONS
} SCOREP_Ipc_Operation;


/**
 * \enum SCOREP_SubstratesRequirementFlags
 * \brief Substrates can define requirements for Score-P. This enables some optimizations.
 * Substrate Plugins pass it to Score-P via SCOREP_SubstratePluginInfo.get_requirement()
 * Internal Plugins pass it to Score-P via SCOREP_Substrates_getRequirement()
 * If this SCOREP_SubstratePluginInfo.get_requirements is not implemented, it is assumed that all values are 0.
 * A description is provided with each of the requirement flags.
 * New requirement flags can be added by just stating them in SCOREP_SubstratesRequirementFlag.
 */
typedef enum SCOREP_Substrates_RequirementFlag
{
    SCOREP_SUBSTRATES_REQUIREMENT_EXPERIMENT_DIRECTORY = 0, /**< Set this to != 0 if you need the experiment directory. If no substrate uses this option, the experiment directory will not be created and no configuration file will be written.*/

    SCOREP_SUBSTRATES_NUM_REQUIREMENT                       /**< Non-ABI used internally  */
} SCOREP_Substrates_RequirementFlag;

/** @} */

#endif /* SCOREP_PUBLICTYPES_H */
