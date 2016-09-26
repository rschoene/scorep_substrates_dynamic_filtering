/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2016,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2015-2016,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 *
 */

#ifndef SCOREP_SUBSTRATE_EVENTS_H
#define SCOREP_SUBSTRATE_EVENTS_H


#include <stdbool.h>

#include <scorep/SCOREP_MetricTypes.h>
#include <scorep/SCOREP_PublicTypes.h>
#include <scorep/SCOREP_PublicHandles.h>


/**
 * Generic void function pointer for substrate functions.
 */
typedef void ( * SCOREP_Substrates_Callback )( void );

struct SCOREP_Location;

/**
 * Substrates need to provide two sets of callbacks for the modes
 * SCOREP_SUBSTATES_RECORDING_ENABLED and SCOREP_SUBSTRATES_RECORDING_DISABLED.
 * This enum is used as an array index.
 */
typedef enum SCOREP_Substrates_Mode
{
    SCOREP_SUBSTRATES_RECORDING_ENABLED = 0,
    SCOREP_SUBSTRATES_RECORDING_DISABLED,

    SCOREP_SUBSTRATES_NUM_MODES
} SCOREP_Substrates_Mode;


/**
 * Substrate events. List every event that is going to be used by the
 * substrate mechanism.
 */
typedef enum SCOREP_Substrates_EventType
{
    SCOREP_EVENT_ENABLE_RECORDING = 0,
    SCOREP_EVENT_DISABLE_RECORDING,
    SCOREP_EVENT_ON_TRACING_BUFFER_FLUSH_BEGIN,
    SCOREP_EVENT_ON_TRACING_BUFFER_FLUSH_END,
    SCOREP_EVENT_ENTER_REGION,
    SCOREP_EVENT_EXIT_REGION,
    SCOREP_EVENT_SAMPLE,
    SCOREP_EVENT_CALLING_CONTEXT_ENTER,
    SCOREP_EVENT_CALLING_CONTEXT_EXIT,
    SCOREP_EVENT_ENTER_REWIND_REGION,
    SCOREP_EVENT_EXIT_REWIND_REGION,
    SCOREP_EVENT_MPI_SEND,
    SCOREP_EVENT_MPI_RECV,
    SCOREP_EVENT_MPI_COLLECTIVE_BEGIN,
    SCOREP_EVENT_MPI_COLLECTIVE_END,
    SCOREP_EVENT_MPI_ISEND_COMPLETE,
    SCOREP_EVENT_MPI_IRECV_REQUEST,
    SCOREP_EVENT_MPI_REQUEST_TESTED,
    SCOREP_EVENT_MPI_REQUEST_CANCELLED,
    SCOREP_EVENT_MPI_ISEND,
    SCOREP_EVENT_MPI_IRECV,
    SCOREP_EVENT_RMA_WIN_CREATE,
    SCOREP_EVENT_RMA_WIN_DESTROY,
    SCOREP_EVENT_RMA_COLLECTIVE_BEGIN,
    SCOREP_EVENT_RMA_COLLECTIVE_END,
    SCOREP_EVENT_RMA_TRY_LOCK,
    SCOREP_EVENT_RMA_ACQUIRE_LOCK,
    SCOREP_EVENT_RMA_REQUEST_LOCK,
    SCOREP_EVENT_RMA_RELEASE_LOCK,
    SCOREP_EVENT_RMA_SYNC,
    SCOREP_EVENT_RMA_GROUP_SYNC,
    SCOREP_EVENT_RMA_PUT,
    SCOREP_EVENT_RMA_GET,
    SCOREP_EVENT_RMA_ATOMIC,
    SCOREP_EVENT_RMA_WAIT_CHANGE,
    SCOREP_EVENT_RMA_OP_COMPLETE_BLOCKING,
    SCOREP_EVENT_RMA_OP_COMPLETE_NON_BLOCKING,
    SCOREP_EVENT_RMA_OP_TEST,
    SCOREP_EVENT_RMA_OP_COMPLETE_REMOTE,
    SCOREP_EVENT_THREAD_ACQUIRE_LOCK,
    SCOREP_EVENT_THREAD_RELEASE_LOCK,
    SCOREP_EVENT_TRIGGER_COUNTER_INT64,
    SCOREP_EVENT_TRIGGER_COUNTER_UINT64,
    SCOREP_EVENT_TRIGGER_COUNTER_DOUBLE,
    SCOREP_EVENT_TRIGGER_PARAMETER_INT64,
    SCOREP_EVENT_TRIGGER_PARAMETER_UINT64,
    SCOREP_EVENT_TRIGGER_PARAMETER_STRING,
    SCOREP_EVENT_THREAD_FORK_JOIN_FORK,
    SCOREP_EVENT_THREAD_FORK_JOIN_JOIN,
    SCOREP_EVENT_THREAD_FORK_JOIN_TEAM_BEGIN,
    SCOREP_EVENT_THREAD_FORK_JOIN_TEAM_END,
    SCOREP_EVENT_THREAD_FORK_JOIN_TASK_CREATE,
    SCOREP_EVENT_THREAD_FORK_JOIN_TASK_SWITCH,
    SCOREP_EVENT_THREAD_FORK_JOIN_TASK_BEGIN,
    SCOREP_EVENT_THREAD_FORK_JOIN_TASK_END,
    SCOREP_EVENT_THREAD_CREATE_WAIT_CREATE,
    SCOREP_EVENT_THREAD_CREATE_WAIT_WAIT,
    SCOREP_EVENT_THREAD_CREATE_WAIT_BEGIN,
    SCOREP_EVENT_THREAD_CREATE_WAIT_END,
    SCOREP_EVENT_TRACK_ALLOC,
    SCOREP_EVENT_TRACK_REALLOC,
    SCOREP_EVENT_TRACK_FREE,

    SCOREP_SUBSTRATES_NUM_EVENTS
} SCOREP_Substrates_EventType;


/**
 * called when enabling the recording on a process and its sub-locations
 * This is triggered by user instrumentation and can not be called when in parallel
 * The default mode is enabled.
 * If the default mode is set to disabled by user instrumentation, an
 * enabled event is called before the substrate is finalized
 *
 * @param location location which creates this event.
 *
 * @param timestamp timestamp for this event
 *
 * @param regionHandle "MEASUREMENT OFF" region
 *
 * @param metricValues synchronous metrics at timestamp
 */
typedef void ( * SCOREP_Substrates_EnableRecordingCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_RegionHandle     regionHandle,
    uint64_t*               metricValues );

/**
 * called when disabling the recording on a process and its sub-locations
 * This is triggered by user instrumentation and can not be called when in parallel
 *
 * @see SCOREP_Substrates_EnableRecordingCb
 */
typedef void ( * SCOREP_Substrates_DisableRecordingCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_RegionHandle     regionHandle,
    uint64_t*               metricValues );

/**
 * called when flushing the tracing buffer of a location
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param regionHandle "TRACE BUFFER FLUSH" region
 *
 * @param metricValues synchronous metrics at timestamp
 */
typedef void ( * SCOREP_Substrates_OnTracingBufferFlushBeginCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_RegionHandle     regionHandle,
    uint64_t*               metricValues );

/**
 * called after flushing the tracing buffer of a location,
 * @see SCOREP_Substrates_OnTracingBufferFlushBeginCb
 */
typedef void ( * SCOREP_Substrates_OnTracingBufferFlushEndCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_RegionHandle     regionHandle,
    uint64_t*               metricValues );

/**
 * called when entering a region via some instrumentation adapter
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param regionHandle region that is entered
 *
 * @param metricValues synchronous metrics at timestamp
 */
typedef void ( * SCOREP_Substrates_EnterRegionCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_RegionHandle     regionHandle,
    uint64_t*               metricValues );

/**
 * called when exiting a region via some instrumentation adapter
 * @see SCOREP_Substrates_EnterRegionCb
 */
typedef void ( * SCOREP_Substrates_ExitRegionCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_RegionHandle     regionHandle,
    uint64_t*               metricValues );

/**
 * called when a sampling adapter interrupts the workload and records a sample
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param callingContext callstack at timestamp
 *
 * @param previousCallingContext calling context of the last SCOREP_Substrates_SampleCb
 *
 * @param unwindDistance number of stack levels changed since the last sample
 *
 * @param interruptGeneratorHandle source that interrupted the workload
 *
 * @param metricValues synchronous metrics at timestamp
 */
typedef void ( * SCOREP_Substrates_SampleCb )(
    struct SCOREP_Location*         location,
    uint64_t                        timestamp,
    SCOREP_CallingContextHandle     callingContext,
    SCOREP_CallingContextHandle     previousCallingContext,
    uint32_t                        unwindDistance,
    SCOREP_InterruptGeneratorHandle interruptGeneratorHandle,
    uint64_t*                       metricValues );

/**
 * called when entering a region via a instrumentation adapter
 * and Score-P is recording calling contexts,
 * alternative to SCOREP_Substrates_EnterRegionCb
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param callingContext callstack at timestamp
 *
 * @param previousCallingContext calling context of the last SCOREP_Substrates_SampleCb
 *
 * @param unwindDistance number of stack levels changed since the last sample
 *
 * @param metricValues synchronous metrics at timestamp
 */
typedef void ( * SCOREP_Substrates_CallingContextEnterCb )(
    struct SCOREP_Location*     location,
    uint64_t                    timestamp,
    SCOREP_CallingContextHandle callingContext,
    SCOREP_CallingContextHandle previousCallingContext,
    uint32_t                    unwindDistance,
    uint64_t*                   metricValues );

/**
 * called when exiting a region via a instrumentation adapter
 * and Score-P is recording calling contexts,
 * alternative to SCOREP_Substrates_ExitRegionCb
 * @see SCOREP_Substrates_CallingContextEnterCb
 */
typedef void ( * SCOREP_Substrates_CallingContextExitCb )(
    struct SCOREP_Location*     location,
    uint64_t                    timestamp,
    SCOREP_CallingContextHandle callingContext,
    SCOREP_CallingContextHandle previousCallingContext,
    uint32_t                    unwindDistance,
    uint64_t*                   metricValues );

/**
 * called when the user adapter enters a rewind region.
 * The recording of the region and any following region information after this
 * should be discarded when the next SCOREP_Substrates_ExitRewindRegionCb for this
 * regionHandle is called with do_rewind == true
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param regionHandle region that is entered
 */
typedef void ( * SCOREP_Substrates_EnterRewindRegionCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_RegionHandle     regionHandle );

/**
 * called when the user adapter exits a rewind region.
 * The recording of the region and any previous regions since
 * SCOREP_Substrates_ExitRewindRegionCb for this regionHandle
 * should be discarded if do_rewind == true
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param regionHandle region that is entered
 *
 * @param doRewind whether to discard previously recorded data or not
 */
typedef void ( * SCOREP_Substrates_ExitRewindRegionCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_RegionHandle     regionHandle,
    bool                    doRewind );

/**
 * called when an MPI_Send is recognized by the MPI adapter
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 * There should be a SCOREP_Substrates_MpiRecvCb call on the location of the
 * destinationRank.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param destinationRank rank that should receive the message
 *
 * @param communicatorHandle communicator handle to which the location rank and the destinationRank belong
 *
 * @param tag provided MPI tag for this message
 *
 * @param bytesSent number of bytes sent with this message
 */
typedef void ( * SCOREP_Substrates_MpiSendCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_MpiRank                   destinationRank,
    SCOREP_InterimCommunicatorHandle communicatorHandle,
    uint32_t                         tag,
    uint64_t                         bytesSent );

/**
 * called when an MPI_Recv is recognized by the MPI adapter
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 * There should be a SCOREP_Substrates_MpiSendCb call on the location of the
 * sourceRank.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param sourceRank rank that sent the message that is to be received
 *
 * @param communicatorHandle communicator handle to which the location rank and the destinationRank belong
 *
 * @param tag provided MPI tag for this message
 *
 * @param bytesSent number of bytes received with this message
 */
typedef void ( * SCOREP_Substrates_MpiRecvCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_MpiRank                   sourceRank,
    SCOREP_InterimCommunicatorHandle communicatorHandle,
    uint32_t                         tag,
    uint64_t                         bytesReceived );

/**
 * Called when an MPI collective is recognized by the MPI adapter before it is started.
 * See also the MPI specifications at https://www.mpi-forum.org/docs/
 * More information on the type is passed with the SCOREP_Substrates_MpiCollectiveEndCb callback
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 */
typedef void ( * SCOREP_Substrates_MpiCollectiveBeginCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp );


/**
 * called when an MPI_Recv is recognized by the MPI adapter
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 * There should be a SCOREP_Substrates_MpiSendCb call on the location of the
 * sourceRank.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param communicatorHandle communicator handle to which the location rank and the destinationRank belong
 *
 * @param rootRank rank that sent the message that is to be received
 *
 * @param tag provided MPI tag for this message
 *
 * @param bytesSent number of bytes received with this message
 */
typedef void ( * SCOREP_Substrates_MpiCollectiveEndCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_InterimCommunicatorHandle communicatorHandle,
    SCOREP_MpiRank                   rootRank,
    SCOREP_CollectiveType            collectiveType,
    uint64_t                         bytesSent,
    uint64_t                         bytesReceived );

/**
 * Called from MPI adapter when an MPI_Isend is completed
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 * Previously there should be a SCOREP_Substrates_MpiIsendCb with the same requestId
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param requestId request ID of the MPI_Isend (see MPI standard)
 */
typedef void ( * SCOREP_Substrates_MpiIsendCompleteCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_MpiRequestId     requestId );

/**
 * Called from MPI adapter when an MPI_Irecv is initialized
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 * Previously there should be a SCOREP_Substrates_MpiIrecvCb with the same requestId
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param requestId request ID of the MPI_Irecv (see MPI standard)
 */
typedef void ( * SCOREP_Substrates_MpiIrecvRequestCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_MpiRequestId     requestId );

/**
 * Called from MPI adapter when the status of a non-blocking communication is tested
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 * Previously there should be an initialization of the non-blocking communication with the same requestId
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param requestId request ID of the non blocking communication to be tested (see MPI standard)
 */
typedef void ( * SCOREP_Substrates_MpiRequestTestedCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_MpiRequestId     requestId );

/**
 * Called from MPI adapter when a non-blocking communication is cancelled
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 * Previously there should be an initialization of the non-blocking communication with the same requestId
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param requestId request ID of the non blocking communication to be canceled (see MPI standard)
 */
typedef void ( * SCOREP_Substrates_MpiRequestCancelledCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_MpiRequestId     requestId );

/**
 * Initialize a non blocking send via MPI
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param destinationRank rank that will receive the message
 *
 * @param communicatorHandle communicator of this location and the target rank
 *
 * @param tag MPI tag
 *
 * @param bytesSent number of sent bytes
 *
 * @param requestId request ID of the non blocking communication to be canceled (see MPI standard)
 */
typedef void ( * SCOREP_Substrates_MpiIsendCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_MpiRank                   destinationRank,
    SCOREP_InterimCommunicatorHandle communicatorHandle,
    uint32_t                         tag,
    uint64_t                         bytesSent,
    SCOREP_MpiRequestId              requestId );

/**
 * Finishes an MpiIrecvRequest
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 *
 * @param location location which sends an MPI message
 *
 * @param timestamp timestamp for this event
 *
 * @param sourceRank rank that sent the message
 *
 * @param communicatorHandle communicator of this location and the source rank
 *
 * @param tag MPI tag
 *
 * @param bytesSent number of sent bytes
 *
 * @param requestId request ID of the non blocking communication to be canceled (see MPI standard)
 */
typedef void ( * SCOREP_Substrates_MpiIrecvCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_MpiRank                   sourceRank,
    SCOREP_InterimCommunicatorHandle communicatorHandle,
    uint32_t                         tag,
    uint64_t                         bytesReceived,
    SCOREP_MpiRequestId              requestId );

/**
 * create a remote memory access window
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param windowHandle previously defined window handle
 */
typedef void ( * SCOREP_Substrates_RmaWinCreateCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle );

/**
 * destroy a remote memory access window
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param windowHandle previously defined and created window handle
 */
typedef void ( * SCOREP_Substrates_RmaWinDestroyCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle );

/**
 * begin a collective operation on an MPI remote memory access window
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 * More information is passed with the next call SCOREP_Substrates_RmaCollectiveEndCb
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 */
typedef void ( * SCOREP_Substrates_RmaCollectiveBeginCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp );

/**
 * end a collective operation on an MPI remote memory access window
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param collectiveOp type of the collective operation
 *
 * @param syncLevel synchronization level
 *
 * @param windowHandle the previously defined and created window handle
 *
 * @param root Root process/rank if there is one
 *
 * @param bytesSent number of bytes sent
 *
 * @param bytesReceived number of bytes received
 */
typedef void ( * SCOREP_Substrates_RmaCollectiveEndCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_CollectiveType         collectiveOp,
    SCOREP_RmaSyncLevel           syncLevel,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint32_t                      root,
    uint64_t                      bytesSent,
    uint64_t                      bytesReceived );

/**
 * An attempt to acquire a lock which turns out negative can be marked
 * with SCOREP_Substrates_RmaTryLockCb. In this case, no release record may follow.
 * With this a series of unsuccessful locking attempts can be identified.
 * If an lock attempt is successful, it is marked with
 * @a SCOREP_Substrates_RmaAcquireLockCb right away instead of a pair of
 * @a SCOREP_Substrates_RmaTryLockCb and @ SCOREP_Substrates_RmaAcquireLockCb.
 * see also the MPI specifications at https://www.mpi-forum.org/docs/
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param windowHandle the previously defined and created window handle
 *
 * @param remote Rank of target in context of window
 *
 * @param lockId Lock id in context of window.
 *
 * @param lockType Type of lock (shared vs. exclusive).
 */
typedef void ( * SCOREP_Substrates_RmaTryLockCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint32_t                      remote,
    uint64_t                      lockId,
    SCOREP_LockType               lockType );

/**
 * Marks the time that a lock is granted. This is the typical situation.
 * It has to be followed by a matching @a SCOREP_Substrates_RmaRequestLockCb record
 * later on.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param windowHandle Memory window.
 *
 * @param remote Rank of target in context of window.
 *
 * @param lockId Lock id in context of window.
 *
 * @param lockType Type of lock (shared vs. exclusive).
 */
typedef void ( * SCOREP_Substrates_RmaAcquireLockCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint32_t                      remote,
    uint64_t                      lockId,
    SCOREP_LockType               lockType );


/**
 * This record marks the time that a request for a lock is issued where
 * the RMA model ensures that the lock is granted eventually without
 * further notification. As of now this is specific for MPI. In this case,
 * the @a SCOREP_RmaAquireLock event is not present.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param win Memory window.
 *
 * @param remote Rank of target in context of window.
 *
 * @param lockId Lock id in context of window.
 *
 * @param lockType Type of lock (shared vs. exclusive).
 */
typedef void ( * SCOREP_Substrates_RmaRequestLockCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint32_t                      remote,
    uint64_t                      lockId,
    SCOREP_LockType               lockType );


/**
 * Marks the time the lock is freed. It contains all fields that are
 * necessary to match it to either an earlier @a SCOREP_Substrates_RmaAcquireLockCb or
 * @a SCOREP_Substrates_RmaRequestLockCb event and is required to follow either of the
 * two.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param windowHandle Memory window.
 *
 * @param remote Rank of target in context of window.
 *
 * @param lockId Lock id in context of window.
 *
 * @param lockType Type of lock (shared vs. exclusive).
 */
typedef void ( * SCOREP_Substrates_RmaReleaseLockCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint32_t                      remote,
    uint64_t                      lockId );

/**
 * This record marks a simple pairwise synchronization.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param windowHandle Memory window.
 *
 * @param remote Rank of target in context of window.
 *
 * @param syncType Type of direct RMA synchronization call
 * (e.g. SCOREP_RMA_SYNC_TYPE_MEMORY, SCOREP_RMA_SYNC_TYPE_NOTIFY_IN,
 * SCOREP_RMA_SYNC_TYPE_NOTIFY_OUT).
 */
typedef void ( * SCOREP_Substrates_RmaSyncCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint32_t                      remote,
    SCOREP_RmaSyncType            syncType );

/**
 * This record marks the synchronization of a sub-group of the locations
 * associated with the given memory window. It needs to be recorded for
 * all participating locations.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param syncLevel Synchronization level.
 *
 * @param windowHandle Memory window.
 *
 * @param group Group of participating processes or threads.
 */
typedef void ( * SCOREP_Substrates_RmaGroupSyncCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_RmaSyncLevel           syncLevel,
    SCOREP_InterimRmaWindowHandle windowHandle,
    SCOREP_GroupHandle            groupHandle );

/**
 * The get and put operations access remote memory addresses. The
 * corresponding get and put records mark when they are issued. The
 * actual start and the completion may happen later.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param windowHandle Memory window.
 *
 * @param remote Rank of target in context of window.
 *
 * @param bytes Number of bytes transferred.
 *
 * @param matchingId Matching number.
 *
 * @note The matching number allows to reference the point of completion
 * of the operation. It will reappear in a completion record on the same
 * location.
 *
 * @{
 */
typedef void ( * SCOREP_Substrates_RmaPutCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint32_t                      remote,
    uint64_t                      bytes,
    uint64_t                      matchingId );

typedef void ( * SCOREP_Substrates_RmaGetCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint32_t                      remote,
    uint64_t                      bytes,
    uint64_t                      matchingId );

/** @} */


/**
 * The atomic RMA operations are similar to the get and put operations.
 * As an additional field they provide the type of operation. Depending
 * on the type, data may be received, sent, or both, therefore, the
 * sizes are specified separately. Matching the local and optionally
 * remote completion works the same way as for get and put operations.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param windowHandle Window.
 *
 * @param remote Rank of target in context of window.
 *
 * @param type Type of atomic operation (see @a SCOREP_RmaAtomicType).
 *
 * @param bytesSent Number of bytes transferred to rmeote target.
 *
 * @param bytesReceived Number of bytes transferred from remote target.
 *
 * @param matchingId Matching number.
 *
 */

typedef void ( * SCOREP_Substrates_RmaAtomicCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint32_t                      remote,
    SCOREP_RmaAtomicType          type,
    uint64_t                      bytesSent,
    uint64_t                      bytesReceived,
    uint64_t                      matchingId );

/**
 * The SCOREP_EVENT_RMA_WAIT_CHANGE event marks a synchronization point that
 * blocks until a remote operation modifies a given memory field. This
 * event marks the beginning of the waiting period. The memory field in
 * question is part of the specified window.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param windowHandle Memory window.
 */
typedef void ( * SCOREP_Substrates_RmaWaitChangeCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle );

/**
 * The completion records mark the end of RMA operations. Local
 * completion for every RMA operation (get, put, or atomic operation)
 * always has to be marked with either @a SCOREP_Substrates_RmaOpCompleteBlockingCb or
 * @a SCOREP_Substrates_RmaOpCompleteNonBlockingCb using the same matching number as
 * the RMA operation record. An RMA operation is blocking when the
 * operation completes locally before leaving the call, for non-blocking
 * operations local completion has to be ensured by a subsequent call.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param win Memory window.
 *
 * @param matchingId Matching number.
 *
 * @{
 */

typedef void ( * SCOREP_Substrates_RmaOpCompleteBlockingCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint64_t                      matchingId );

typedef void ( * SCOREP_Substrates_RmaOpCompleteNonBlockingCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint64_t                      matchingId );

/** @} */


/**
 * This record indicates a test for completion. It is only useful for
 * non-blocking RMA calls where the API supports such a test. The test
 * record stands for a negative outcome, otherwise a completion record
 * is written (see @a SCOREP_Substrates_RmaOpCompleteRemoteCb).
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param win Memory window.
 *
 * @param matchingId Matching number.
 */
typedef void ( * SCOREP_Substrates_RmaOpTestCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint64_t                      matchingId );


/**
 * An optional remote completion point can be specified with
 * SCOREP_Substrates_RmaOpCompleteRemoteCb. It is recorded on the same location as
 * the RMA operation itself. Again, multiple RMA operations may map to
 * the same SCOREP_Substrates_RmaOpCompleteRemoteCb. The target locations are not
 * explicitly specified but implicitly as all those that were referenced
 * in matching RMA operations.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param win Memory window.
 *
 * @param matchingId Matching number.
 */
typedef void ( * SCOREP_Substrates_RmaOpCompleteRemoteCb )(
    struct SCOREP_Location*       location,
    uint64_t                      timestamp,
    SCOREP_InterimRmaWindowHandle windowHandle,
    uint64_t                      matchingId );


/**
 * Process a thread acquire/release lock event in the measurement system.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param paradigm the underlying parallelization paradigm of the lock
 *
 * @param lockId A unique ID to identify the lock, maintained by
 *               the caller.
 * @param acquisitionOrder A monotonically increasing id to determine the
 *                         order of lock acquisitions. Same for corresponding
 *                         acquire-release events.
 *
 * @{
 */
typedef void ( * SCOREP_Substrates_ThreadAcquireLockCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_ParadigmType     paradigm,
    uint32_t                lockId,
    uint32_t                acquisitionOrder );


typedef void ( * SCOREP_Substrates_ThreadReleaseLockCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_ParadigmType     paradigm,
    uint32_t                lockId,
    uint32_t                acquisitionOrder );

/** @} */

/**
 * Trigger a counter, which represents more or less a metric
 * @see also SCOREP_User_TriggerMetricInt64 SCOREP_User_TriggerMetricUint64 SCOREP_User_TriggerMetricDouble
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param counterHandle previously defined counter handle
 *
 * @param value value of the counter when triggered. The datatype depends on the called function
 *
 * @{
 */
typedef void ( * SCOREP_Substrates_TriggerCounterInt64Cb )(
    struct SCOREP_Location*  location,
    uint64_t                 timestamp,
    SCOREP_SamplingSetHandle counterHandle,
    int64_t                  value );

typedef void ( * SCOREP_Substrates_TriggerCounterUint64Cb )(
    struct SCOREP_Location*  location,
    uint64_t                 timestamp,
    SCOREP_SamplingSetHandle counterHandle,
    uint64_t                 value );

typedef void ( * SCOREP_Substrates_TriggerCounterDoubleCb )(
    struct SCOREP_Location*  location,
    uint64_t                 timestamp,
    SCOREP_SamplingSetHandle counterHandle,
    double                   value );
/** @} */


/**
 * Trigger a user defined parameter with a specific value
 * @see also SCOREP_User_ParameterInt64 SCOREP_User_ParameterUint64
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param parameterHandle previously defined parameter handle
 *
 * @param value value of the parameter when triggered. The datatype depends on the called function
 *
 * @{
 */
typedef void ( * SCOREP_Substrates_TriggerParameterInt64Cb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_ParameterHandle  parameterHandle,
    int64_t                 value );

typedef void ( * SCOREP_Substrates_TriggerParameterUint64Cb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_ParameterHandle  parameterHandle,
    uint64_t                value );
/** @} */

/**
 * Trigger a user defined parameter with a specific value
 * @see also SCOREP_User_ParameterString
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param parameterHandle previously defined parameter handle
 *
 * @param string_handle previously defined string when the parameter is triggered.
 *
 */
typedef void ( * SCOREP_Substrates_TriggerParameterStringCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_ParameterHandle  parameterHandle,
    SCOREP_StringHandle     string_handle );

/**
 * called from threading instrumentation adapters before a thread team is forked, e.g., before an OpenMP parallel region
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param paradigm threading paradigm
 *
 * @param nRequestedThreads number of threads to be forked.
 *  Note that this does not necessarily represent actual threads
 *  but threads can also be reused, e..g, in OpenMP runtimes.
 *
 * @param forkSequenceCount an increasing number, unique for each process that
 *   allows to identify a parallel region
 *
 */

typedef void ( * SCOREP_Substrates_ThreadForkJoinForkCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_ParadigmType     paradigm,
    uint32_t                nRequestedThreads,
    uint32_t                forkSequenceCount );

/**
 * called from threading instrumentation after a thread team is joined, e.g., after an OpenMP parallel region
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param paradigm threading paradigm
 *
 */
typedef void ( * SCOREP_Substrates_ThreadForkJoinJoinCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    SCOREP_ParadigmType     paradigm );

/**
 * called from threading instrumentation after a thread team is created/before it is joined.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param paradigm threading paradigm
 *
 * @param threadTeam previously defined thread team
 *
 * @{
 */
typedef void ( * SCOREP_Substrates_ThreadForkJoinTeamBeginCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam );

typedef void ( * SCOREP_Substrates_ThreadForkJoinTeamEndCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam );

/** @} */



/**
 * Process a task create event in the measurement system.
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param paradigm         One of the predefined threading models.
 *
 * @param threadTeam previously defined thread team
 *
 * @param threadId         Id of the this thread within the team of
 *                         threads that constitute the parallel region.
 *
 * @param generationNumber The sequence number for this task. Each task
 *                         gets a thread private generation number of the
 *                         creating thread attached. Combined with the
 *                         @a threadId, this constitutes a unique task ID
 *                         inside the parallel region.
 *
 */
typedef void ( * SCOREP_Substrates_ThreadForkJoinTaskCreateCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam,
    uint32_t                         threadId,
    uint32_t                         generationNumber );

/**
 * Process a task switch event
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param metricValues synchronous metrics
 *
 * @param paradigm         One of the predefined threading models.
 *
 * @param threadTeam previously defined thread team
 *
 * @param threadId         Id of the this thread within the team of
 *                         threads that constitute the parallel region.
 *
 * @param generationNumber The sequence number for this task. Each task
 *                         gets a thread private generation number of the
 *                         creating thread attached. Combined with the
 *                         @a threadId, this constitutes a unique task ID
 *                         inside the parallel region.
 *
 * @param task             A handle to the resumed task.
 */

typedef void ( * SCOREP_Substrates_ThreadForkJoinTaskSwitchCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    uint64_t*                        metricValues,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam,
    uint32_t                         threadId,
    uint32_t                         generationNumber,
    SCOREP_TaskHandle                taskHandle );

/**
 * Process a task begin/end event
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param metricValues synchronous metrics
 *
 * @param paradigm         One of the predefined threading models.
 *
 * @param regionHandle     Region handle of the task region.
 *
 * @param threadTeam previously defined thread team
 *
 * @param threadId         Id of the this thread within the team of
 *                         threads that constitute the parallel region.
 *
 * @param generationNumber The sequence number for this task. Each task created
 *                         gets a thread private generation number attached.
 *                         Combined with the @a threadId, this constitutes a
 *                         unique task ID inside the parallel region.
 *
 * @param taskHandle       A handle to the executed task
 *
 * @{
 */
typedef void ( * SCOREP_Substrates_ThreadForkJoinTaskBeginCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_RegionHandle              regionHandle,
    uint64_t*                        metricValues,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam,
    uint32_t                         threadId,
    uint32_t                         generationNumber,
    SCOREP_TaskHandle                taskHandle );

typedef void ( * SCOREP_Substrates_ThreadForkJoinTaskEndCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_RegionHandle              regionHandle,
    uint64_t*                        metricValues,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam,
    uint32_t                         threadId,
    uint32_t                         generationNumber,
    SCOREP_TaskHandle                taskHandle );

/** @} */

/**
 * process a thread event for a create/wait thread model instrumentation adapter e.g., pthreads
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param paradigm         One of the predefined threading models.
 *
 * @param threadTeam previously defined thread team
 *
 * @param createSequenceCount a process unique increasing number that is increased at every
 * SCOREP_Substrates_ThreadCreateWaitCreateCb and SCOREP_Substrates_ThreadForkJoinForkCb
 */

typedef void ( * SCOREP_Substrates_ThreadCreateWaitCreateCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam,
    uint32_t                         createSequenceCount );

typedef void ( * SCOREP_Substrates_ThreadCreateWaitWaitCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam,
    uint32_t                         createSequenceCount );

typedef void ( * SCOREP_Substrates_ThreadCreateWaitBeginCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam,
    uint32_t                         createSequenceCount );

typedef void ( * SCOREP_Substrates_ThreadCreateWaitEndCb )(
    struct SCOREP_Location*          location,
    uint64_t                         timestamp,
    SCOREP_ParadigmType              paradigm,
    SCOREP_InterimCommunicatorHandle threadTeam,
    uint32_t                         createSequenceCount );

/** @} */

/**
 * Event for allocating memory using malloc/calloc
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param addrAllocated allocated address, should be converted to void*
 *
 * @param bytesAllocated number of bytes allocated
 *
 * @param substrateData ???
 *
 * @param bytesAllocatedMetric ???
 *
 * @param bytesAllocatedProcess total number of bytes allocated in the process
 *
 */
typedef void ( * SCOREP_Substrates_TrackAllocCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    uint64_t                addrAllocated,
    size_t                  bytesAllocated,
    void*                   substrateData[],
    size_t                  bytesAllocatedMetric,
    size_t                  bytesAllocatedProcess );

/**
 * Event for allocating/freeing memory using realloc
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param oldAddr address passed to realloc
 *
 * @param oldBytesAllocated size allocated to oldAddr before calling realloc
 *
 * @param oldSubstrateData ???
 *
 * @param newAddr address gained from realloc
 *
 * @param newBytesAllocated difference between previously allocated memory size and new allocated memory size, can be nagative if new size < old size
 *
 * @param newSubstrateData ???
 *
 * @param bytesAllocatedMetric ???
 *
 * @param bytesAllocatedProcess total number of bytes allocated in the process
 *
 */
typedef void ( * SCOREP_Substrates_TrackReallocCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    uint64_t                oldAddr,
    size_t                  oldBytesAllocated,
    void*                   oldSubstrateData[],
    uint64_t                newAddr,
    size_t                  newBytesAllocated,
    void*                   newSubstrateData[],
    size_t                  bytesAllocatedMetric,
    size_t                  bytesAllocatedProcess );

/**
 * Event for freeing memory using free
 *
 * @param location location which creates this event
 *
 * @param timestamp timestamp for this event
 *
 * @param addrFreed address passed to free
 *
 * @param bytesFreed number of bytes freed
 *
 * @param substrateData ???
 *
 * @param bytesAllocatedMetric ???
 *
 * @param bytesAllocatedProcess total number of bytes allocated in the process
 *
 */
typedef void ( * SCOREP_Substrates_TrackFreeCb )(
    struct SCOREP_Location* location,
    uint64_t                timestamp,
    uint64_t                addrFreed,
    size_t                  bytesFreed,
    void*                   substrateData[],
    size_t                  bytesAllocatedMetric,
    size_t                  bytesAllocatedProcess );



#endif /* SCOREP_SUBSTRATE_EVENTS_H */
