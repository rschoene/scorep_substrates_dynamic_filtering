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

#ifndef SCOREP_PUBLICTYPES_H
#define SCOREP_PUBLICTYPES_H

#include <stdint.h>


/** ingroup SCOREP_Types
    @{
 */

/**
 * Attribute type.
 */
typedef enum SCOREP_AttributeType
{
    SCOREP_ATTRIBUTE_TYPE_INT8,
    SCOREP_ATTRIBUTE_TYPE_INT16,
    SCOREP_ATTRIBUTE_TYPE_INT32,
    SCOREP_ATTRIBUTE_TYPE_INT64,
    SCOREP_ATTRIBUTE_TYPE_UINT8,
    SCOREP_ATTRIBUTE_TYPE_UINT16,
    SCOREP_ATTRIBUTE_TYPE_UINT32,
    SCOREP_ATTRIBUTE_TYPE_UINT64,
    SCOREP_ATTRIBUTE_TYPE_FLOAT,
    SCOREP_ATTRIBUTE_TYPE_DOUBLE,
    SCOREP_ATTRIBUTE_TYPE_STRING,
    SCOREP_ATTRIBUTE_TYPE_ATTRIBUTE,
    SCOREP_ATTRIBUTE_TYPE_LOCATION,
    SCOREP_ATTRIBUTE_TYPE_REGION,
    SCOREP_ATTRIBUTE_TYPE_GROUP,
    SCOREP_ATTRIBUTE_TYPE_METRIC,
    SCOREP_ATTRIBUTE_TYPE_INTERIM_COMMUNICATOR,
    SCOREP_ATTRIBUTE_TYPE_PARAMETER,
    SCOREP_ATTRIBUTE_TYPE_INTERIM_RMA_WINDOW,
    SCOREP_ATTRIBUTE_TYPE_SOURCE_CODE_LOCATION,
    SCOREP_ATTRIBUTE_TYPE_CALLING_CONTEXT,
    SCOREP_ATTRIBUTE_TYPE_INTERRUPT_GENERATOR
} SCOREP_AttributeType;


/**
 * Opaque handle to memory that can be easily moved between
 * processes. Used for definitions as the have to be moved during
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
 * Symbolic constant representing an invalid or unknown I/O file group
 * definition.
 */
#define SCOREP_INVALID_IOFILE_GROUP SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown I/O file definition.
 */
#define SCOREP_INVALID_IOFILE SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown marker group
 * definition.
 */
#define SCOREP_INVALID_MARKER_GROUP SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown marker definition.
 */
#define SCOREP_INVALID_MARKER SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown group
 * definition.
 */
#define SCOREP_INVALID_GROUP SCOREP_MOVABLE_NULL

/**
 * Symbolic constant representing an invalid or unknown local MPI communicator
 * definition.
 */
#define SCOREP_INVALID_INTERIM_COMMUNICATOR SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown MPI communicator
 * definition.
 */
#define SCOREP_INVALID_COMMUNICATOR SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown MPI window definition.
 */
#define SCOREP_INVALID_INTERIM_RMA_WINDOW SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown MPI window definition.
 */
#define SCOREP_INVALID_RMA_WINDOW SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown MPI cartesion topology
 * definition.
 */
#define SCOREP_INVALID_CART_TOPOLOGY SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown MPI cartesion coordinates
 * definition.
 */
#define SCOREP_INVALID_CART_COORDS SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown callpath definition.
 */
#define SCOREP_INVALID_CALLPATH SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown parameter definition.
 */
#define SCOREP_INVALID_PARAMETER SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown string definition.
 */
#define SCOREP_INVALID_STRING SCOREP_MOVABLE_NULL
/**
 * Symbolic constant representing an invalid or unknown location definition.
 */
#define SCOREP_INVALID_LOCATION SCOREP_MOVABLE_NULL

/**
 * Symbolic constant representing an invalid or unknown location group definition.
 */
#define SCOREP_INVALID_LOCATION_GROUP SCOREP_MOVABLE_NULL

/**
 * Symbolic constant representing an invalid or unknown system tree node
 */
#define SCOREP_INVALID_SYSTEM_TREE_NODE SCOREP_MOVABLE_NULL

/**
 * Symbolic constant representing a filtered region
 */
#define SCOREP_FILTERED_REGION ( ( SCOREP_RegionHandle ) - 1 )

/**
 * Symbolic constant representing an invalid or unknown system tree node property
 */
#define SCOREP_INVALID_SYSTEM_TREE_NODE_PROPERTY SCOREP_MOVABLE_NULL

/**
 * Symbolic constant representing an invalid or unknown metric class definition.
 */
#define SCOREP_INVALID_SAMPLING_SET_RECORDER SCOREP_MOVABLE_NULL

/**
 * Symbolic constant representing an invalid or unknown meta data entry.
 */
#define SCOREP_INVALID_LOCATION_PROPERTY SCOREP_MOVABLE_NULL

/**
 * Symbolic constant representing an invalid or unknown calling context entry.
 */
#define SCOREP_INVALID_CALLING_CONTEXT SCOREP_MOVABLE_NULL


/**
 * Symbolic constant representing an invalid or unknown interrupt generator entry.
 */
#define SCOREP_INVALID_INTERRUPT_GENERATOR SCOREP_MOVABLE_NULL


/**
 * Types to be used in defining a region (SCOREP_Definitions_NewRegion()). In order to
 * track the origin of a region definition, the adapter needs to provide @e
 * his type.
 *
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


/* Keep MPI first after the non-parallel paradigms */
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
 *  Known flags for parallel paradigms.
 *
 *  Flags are essential boolean typed SCOREP_ParadigmProperty and thus
 *  exists as convenience.
 */
typedef enum SCOREP_ParadigmFlags
{
    /**
     *  Attests that the paradigm is purely RMA based.
     *
     *  As the definitions structure for RMA windows is based on communicators,
     *  these additional definitions don't reflect the actuall paradigm.
     *  By setting this flag for the paradigm it can therefore attest to the
     *  measurement data reader that the additional communicator definitions
     *  are not used.
     */
    SCOREP_PARADIGM_FLAG_RMA_ONLY = ( 1 << 0 ),

    /** Value for empty flags. */
    SCOREP_PARADIGM_FLAG_NONE     = 0
} SCOREP_ParadigmFlags;


/**
 *  Known properties for parallel paradigms.
 */
typedef enum SCOREP_ParadigmProperty
{
    /** Template for communicators without an explicit name.
     *  Must contain the string "${id}" to make each name unique.
     *  Mostly only useful if the paradigm allows user created communicators.
     *  Value is of type SCOREP_StringHandle.
     */
    SCOREP_PARADIGM_PROPERTY_COMMUNICATOR_TEMPLATE,

    /** Template for RMA windows without an explicit name.
     *  Must contain the string "${id}" to make each name unique.
     *  Mostly only useful if the paradigm allows user created rma windows.
     *  Value is of type SCOREP_StringHandle.
     */
    SCOREP_PARADIGM_PROPERTY_RMA_WINDOW_TEMPLATE,

    SCOREP_INVALID_PARADIGM_PROPERTY /**< For internal use only. */
} SCOREP_ParadigmProperty;


/**
 * Types to be used in defining the occurrence of a sampling set.
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
 * Types to be used in defining the scope of a scoped sampling set.
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
 * Types to be used in defining a location group (SCOREP_Definitions_NewLocationGroup()).
 *
 */
#define SCOREP_LOCATION_GROUP_TYPES \
    SCOREP_LOCATION_GROUP_TYPE( PROCESS, "process" )

typedef enum SCOREP_LocationGroupType
{
    #define SCOREP_LOCATION_GROUP_TYPE( NAME, name_string ) \
    SCOREP_LOCATION_GROUP_TYPE_ ## NAME,

    SCOREP_LOCATION_GROUP_TYPES

    #undef SCOREP_LOCATION_GROUP_TYPE

    SCOREP_INVALID_LOCATION_GROUP_TYPE /**< For internal use only. */
} SCOREP_LocationGroupType;


/**
 * Types to be used in defining a group (SCOREP_Definitions_NewGroup()).
 *
 */
typedef enum SCOREP_GroupType
{
    SCOREP_GROUP_UNKNOWN             = 0,
    SCOREP_GROUP_LOCATIONS           = 1,
    SCOREP_GROUP_REGIONS             = 2,
    SCOREP_GROUP_METRIC              = 3,

    SCOREP_GROUP_MPI_LOCATIONS       = 4,
    SCOREP_GROUP_MPI_GROUP           = 5,
    SCOREP_GROUP_MPI_SELF            = 6,

    SCOREP_GROUP_OPENMP_LOCATIONS    = 7,
    SCOREP_GROUP_OPENMP_THREAD_TEAM  = 8,

    SCOREP_GROUP_CUDA_LOCATIONS      = 9,
    SCOREP_GROUP_CUDA_GROUP          = 10,

    SCOREP_GROUP_SHMEM_LOCATIONS     = 11,
    SCOREP_GROUP_SHMEM_GROUP         = 12,
    SCOREP_GROUP_SHMEM_SELF          = 13,

    SCOREP_GROUP_PTHREAD_LOCATIONS   = 14,
    SCOREP_GROUP_PTHREAD_THREAD_TEAM = 15,

    SCOREP_GROUP_OPENCL_LOCATIONS    = 16,
    SCOREP_GROUP_OPENCL_GROUP        = 17,

    SCOREP_INVALID_GROUP_TYPE /**< For internal use only. */
} SCOREP_GroupType;



/**
 * Types to be used in defining a region (SCOREP_Definitions_NewRegion()). These types
 * are currently not used inside the measurement system. This @e may change in
 * future if we are going to implement phases/dynamic regions etc. inside the
 * measurement system as opposed to inside the adapters or as a postprocessing
 * step. The names should be self explanatory; most of them are already used
 * (whith a different prefix) in VampiTrace and Scalasca.
 *
 * @todo remove SCOREP_REGION_UNKNOWN
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
 * Types to be used in defining a parameter for parameter based profiling
 * (SCOREP_Definitions_NewParameter()).
 *
 */
typedef enum SCOREP_ParameterType
{
    SCOREP_PARAMETER_INT64,
    SCOREP_PARAMETER_UINT64,
    SCOREP_PARAMETER_STRING,

    SCOREP_INVALID_PARAMETER_TYPE /**< For internal use only. */
} SCOREP_ParameterType;

/**
 * Types to specify the used collectives in calls to @a SCOREP_MpiCollectiveBegin
 * and @a SCOREP_RmaCollectiveBegin.
 */
typedef enum SCOREP_CollectiveType
{
    SCOREP_COLLECTIVE_BARRIER,
    SCOREP_COLLECTIVE_BROADCAST,
    SCOREP_COLLECTIVE_GATHER,
    SCOREP_COLLECTIVE_GATHERV,
    SCOREP_COLLECTIVE_SCATTER,
    SCOREP_COLLECTIVE_SCATTERV,
    SCOREP_COLLECTIVE_ALLGATHER,
    SCOREP_COLLECTIVE_ALLGATHERV,
    SCOREP_COLLECTIVE_ALLTOALL,
    SCOREP_COLLECTIVE_ALLTOALLV,
    SCOREP_COLLECTIVE_ALLTOALLW,
    SCOREP_COLLECTIVE_ALLREDUCE,
    SCOREP_COLLECTIVE_REDUCE,
    SCOREP_COLLECTIVE_REDUCE_SCATTER,
    SCOREP_COLLECTIVE_REDUCE_SCATTER_BLOCK,
    SCOREP_COLLECTIVE_SCAN,
    SCOREP_COLLECTIVE_EXSCAN,
    SCOREP_COLLECTIVE_CREATE_HANDLE,
    SCOREP_COLLECTIVE_DESTROY_HANDLE,
    SCOREP_COLLECTIVE_ALLOCATE,
    SCOREP_COLLECTIVE_DEALLOCATE,
    SCOREP_COLLECTIVE_CREATE_HANDLE_AND_ALLOCATE,
    SCOREP_COLLECTIVE_DESTROY_HANDLE_AND_DEALLOCATE
} SCOREP_CollectiveType;



/**
 * Type of MPI Ranks. Type of MPI ranks always int.
 */
typedef int SCOREP_MpiRank;


/**
 * Type of a MPI Non-blocking communication request id.
 */
typedef uint64_t SCOREP_MpiRequestId;


/**
 * @def SCOREP_MPI_INVALID_RANK
 * Defines an invalid MPI rank.
 */
#define SCOREP_MPI_INVALID_RANK -1


/**
 * Symbolic constant representing an invalid or unknown rank.
 * @see SCOREP_MpiCollective()
 */
#define SCOREP_INVALID_ROOT_RANK -1



#define SCOREP_RMA_SYNC_TYPES \
    SCOREP_RMA_SYNC_TYPE( MEMORY, memory, "memory" )             /* Synchronize memory copy. */ \
    SCOREP_RMA_SYNC_TYPE( NOTIFY_IN, notify_in, "notify in" )    /* Incoming remote notification. */ \
    SCOREP_RMA_SYNC_TYPE( NOTIFY_OUT, notify_out, "notify out" ) /* Outgoing remote notification. */

/**
 * Type of direct RMA synchronization call.
 */
typedef enum SCOREP_RmaSyncType
{
#define SCOREP_RMA_SYNC_TYPE( upper, lower, name )  SCOREP_RMA_SYNC_TYPE_ ## upper,
    SCOREP_RMA_SYNC_TYPES
    #undef SCOREP_RMA_SYNC_TYPE

    SCOREP_INVALID_RMA_SYNC_TYPE /**< For internal use only. */
} SCOREP_RmaSyncType;


/*
 * NONE: No process synchronization or access completion (e.g.,
 * MPI_Win_post.
 *
 * PROCESS: Synchronize processes (e.g., MPI_Win_create/free)
 *
 * MEMORY: Complete memory accesses (e.g., MPI_Win_complete, MPI_Win_wait)
 */
#define SCOREP_RMA_SYNC_LEVELS \
    SCOREP_RMA_SYNC_LEVEL( NONE, none, "none", 0 ) \
    SCOREP_RMA_SYNC_LEVEL( PROCESS, process, "process", 1 << 0 ) \
    SCOREP_RMA_SYNC_LEVEL( MEMORY, memory, "memory", 1 << 1 )


/**
 * Types to be used by RMA records to be passed
 * to SCOREP_Rma*() functions.
 */
typedef enum SCOREP_RmaSyncLevel
{
#define SCOREP_RMA_SYNC_LEVEL( upper, lower, name, value ) \
    SCOREP_RMA_SYNC_LEVEL_ ## upper = value,

    SCOREP_RMA_SYNC_LEVELS
#undef SCOREP_RMA_SYNC_LEVEL
} SCOREP_RmaSyncLevel;


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

#define SCOREP_RMA_ATOMIC_TYPES \
    SCOREP_RMA_ATOMIC_TYPE( ACCUMULATE, accumulate,      "accumulate" )   \
    SCOREP_RMA_ATOMIC_TYPE( INCREMENT, increment,       "increment" )     \
    SCOREP_RMA_ATOMIC_TYPE( TEST_AND_SET, test_and_set,    "test and set" ) \
    SCOREP_RMA_ATOMIC_TYPE( COMPARE_AND_SWAP, compare_and_swap, "compare and swap" ) \
    SCOREP_RMA_ATOMIC_TYPE( SWAP, swap, "swap" ) \
    SCOREP_RMA_ATOMIC_TYPE( FETCH_AND_ADD, fetch_and_add, "fetch and add" ) \
    SCOREP_RMA_ATOMIC_TYPE( FETCH_AND_INCREMENT, fetch_and_increment, "fetch and increment" ) \
    SCOREP_RMA_ATOMIC_TYPE( ADD, add, "add" )


/**
 * RMA Atomic Operation Type.
 */
typedef enum SCOREP_RmaAtomicType
{
#define SCOREP_RMA_ATOMIC_TYPE( upper, lower, name ) SCOREP_RMA_ATOMIC_TYPE_ ## upper,
    SCOREP_RMA_ATOMIC_TYPES
    #undef SCOREP_RMA_ATOMIC_TYPE

    SCOREP_INVALID_RMA_ATOMIC_TYPE
} SCOREP_RmaAtomicType;

/**
 * Task Handle
 */
typedef struct SCOREP_Task* SCOREP_TaskHandle;


/**@}*/

#endif /* SCOREP_PUBLICTYPES_H */
