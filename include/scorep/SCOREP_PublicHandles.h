/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2011,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2011,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2011, 2015-2016,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2011,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2011,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2011,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2011,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */

#ifndef SCOREP_PUBLIC_HANDLES_H
#define SCOREP_PUBLIC_HANDLES_H



/**
 * @file
 *
 *
 */


#include <scorep/SCOREP_PublicTypes.h>


/**
 * Substrate events. List every event that is going to be used by the
 * substrate mechanism.
 */
typedef enum SCOREP_HandleType
{
    SCOREP_HANDLE_TYPE_ANY = 0,
    SCOREP_HANDLE_TYPE_ATTRIBUTE,
    SCOREP_HANDLE_TYPE_CALLING_CONTEXT,
    SCOREP_HANDLE_TYPE_CALLPATH,
    SCOREP_HANDLE_TYPE_COMMUNICATOR,
    SCOREP_HANDLE_TYPE_GROUP,
    SCOREP_HANDLE_TYPE_INTERIM_COMMUNICATOR,
    SCOREP_HANDLE_TYPE_INTERIM_RMA_WINDOW,
    SCOREP_HANDLE_TYPE_INTERRUPT_GENERATOR,
    SCOREP_HANDLE_TYPE_LOCATION,
    SCOREP_HANDLE_TYPE_LOCATION_GROUP,
    SCOREP_HANDLE_TYPE_LOCATION_PROPERTY,
    SCOREP_HANDLE_TYPE_METRIC,
    SCOREP_HANDLE_TYPE_MPI_CARTESIAN_TOPOLOGY,
    SCOREP_HANDLE_TYPE_MPI_CARTESIAN_COORDS,
    SCOREP_HANDLE_TYPE_PARAMETER,
    SCOREP_HANDLE_TYPE_PROPERTY,
    SCOREP_HANDLE_TYPE_REGION,
    SCOREP_HANDLE_TYPE_RMA_WINDOW,
    SCOREP_HANDLE_TYPE_SAMPLING_SET,
    SCOREP_HANDLE_TYPE_SAMPLING_SET_RECORDER,
    SCOREP_HANDLE_TYPE_SOURCE_CODE_LOCATION,
    SCOREP_HANDLE_TYPE_SOURCE_FILE,
    SCOREP_HANDLE_TYPE_STRING,
    SCOREP_HANDLE_TYPE_SYSTEM_TREE_NODE,
    SCOREP_HANDLE_TYPE_SYSTEM_TREE_NODE_PROPERTY,

    SCOREP_HANDLE_TYPE_NUM_HANDLES
} SCOREP_HandleType;

typedef SCOREP_AnyHandle SCOREP_AttributeHandle;

typedef SCOREP_AnyHandle SCOREP_CallingContextHandle;

typedef SCOREP_AnyHandle SCOREP_CallpathHandle;

typedef SCOREP_AnyHandle SCOREP_CommunicatorHandle;

typedef SCOREP_AnyHandle SCOREP_GroupHandle;

typedef SCOREP_AnyHandle SCOREP_InterimCommunicatorHandle;

typedef SCOREP_AnyHandle SCOREP_InterruptGeneratorHandle;

typedef SCOREP_AnyHandle SCOREP_InterimRmaWindowHandle;

typedef SCOREP_AnyHandle SCOREP_LocationHandle;

typedef SCOREP_AnyHandle SCOREP_LocationGroupHandle;

typedef SCOREP_AnyHandle SCOREP_LocationPropertyHandle;

typedef SCOREP_AnyHandle SCOREP_MPICartesianTopologyHandle;

typedef SCOREP_AnyHandle SCOREP_MPICartesianCoordsHandle;

typedef SCOREP_AnyHandle SCOREP_ParameterHandle;

typedef SCOREP_AnyHandle SCOREP_PropertyHandle;

typedef SCOREP_AnyHandle SCOREP_RmaWindowHandle;

typedef SCOREP_AnyHandle SCOREP_SamplingSetRecorderHandle;

typedef SCOREP_AnyHandle SCOREP_SourceCodeLocationHandle;

typedef SCOREP_AnyHandle SCOREP_StringHandle;

typedef SCOREP_AnyHandle SCOREP_SystemTreeNodeHandle;

typedef SCOREP_AnyHandle SCOREP_SystemTreeNodePropertyHandle;
















#endif /* SCOREP_PUBLIC_HANDLES_H */
