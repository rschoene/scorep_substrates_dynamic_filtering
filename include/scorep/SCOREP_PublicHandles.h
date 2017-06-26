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
 * @file       SCOREP_PublicHandles.h
 *
 * @brief Description of definition handles.
 * This header defines an enumeration to map SCOREP_AnyHandle to specific handles.
 * It also contains some of these handle definitions.
 */

#include <scorep/SCOREP_PublicTypes.h>


/**
 * handle types, lists all handle types that are supported.
 */
typedef enum SCOREP_HandleType
{
    SCOREP_HANDLE_TYPE_ANY = 0,                   /** The handle type is not defined/invalid */
    SCOREP_HANDLE_TYPE_ATTRIBUTE,                 /** The handle type is SCOREP_AttributeHandle (defined in src/measurement/definitions/include/SCOREP_DefinitionHandles.h) */
    SCOREP_HANDLE_TYPE_CALLING_CONTEXT,           /** The handle type is SCOREP_CallingContextHandle */
    SCOREP_HANDLE_TYPE_CALLPATH,                  /** The handle type is SCOREP_CallpathHandle */
    SCOREP_HANDLE_TYPE_GROUP,                     /** The handle type is SCOREP_GroupHandle */
    SCOREP_HANDLE_TYPE_INTERIM_COMMUNICATOR,      /** The handle type is SCOREP_InterimCommunicatorHandle */
    SCOREP_HANDLE_TYPE_INTERRUPT_GENERATOR,       /** The handle type is SCOREP_InterruptGeneratorHandle */
    SCOREP_HANDLE_TYPE_LOCATION,                  /** The handle type is SCOREP_LocationHandle */
    SCOREP_HANDLE_TYPE_LOCATION_GROUP,            /** The handle type is SCOREP_LocationGroupHandle */
    SCOREP_HANDLE_TYPE_LOCATION_PROPERTY,         /** The handle type is SCOREP_LocationPropertyHandle */
    SCOREP_HANDLE_TYPE_METRIC,                    /** The handle type is SCOREP_MetricHandle (defined in SCOREP_PublicTypes.h) */
    SCOREP_HANDLE_TYPE_PARADIGM,                  /** The handle type is SCOREP_ParadigmHandle (defined in SCOREP_PublicTypes.h) */
    SCOREP_HANDLE_TYPE_PARAMETER,                 /** The handle type is SCOREP_ParameterHandle */
    SCOREP_HANDLE_TYPE_PROPERTY,                  /** The handle type is SCOREP_PropertyHandle */
    SCOREP_HANDLE_TYPE_REGION,                    /** The handle type is SCOREP_RegionHandle (defined in SCOREP_PublicTypes.h) */
    SCOREP_HANDLE_TYPE_RMA_WINDOW,                /** The handle type is SCOREP_RmaWindowHandle */
    SCOREP_HANDLE_TYPE_SAMPLING_SET,              /** The handle type is SCOREP_SamplingSetHandle (defined in SCOREP_PublicTypes.h) */
    SCOREP_HANDLE_TYPE_SAMPLING_SET_RECORDER,     /** The handle type is SCOREP_SamplingSetRecorderHandle */
    SCOREP_HANDLE_TYPE_SOURCE_CODE_LOCATION,      /** The handle type is SCOREP_SourceCodeLocationHandle */
    SCOREP_HANDLE_TYPE_SOURCE_FILE,               /** The handle type is SCOREP_SourceFileHandle (defined in SCOREP_PublicTypes.h) */
    SCOREP_HANDLE_TYPE_STRING,                    /** The handle type is SCOREP_StringHandle */
    SCOREP_HANDLE_TYPE_SYSTEM_TREE_NODE,          /** The handle type is SCOREP_SystemTreeNodeHandle */
    SCOREP_HANDLE_TYPE_SYSTEM_TREE_NODE_PROPERTY, /** The handle type is SCOREP_SystemTreeNodePropertyHandle */

    SCOREP_HANDLE_TYPE_NUM_HANDLES                /** Not ABI  */
} SCOREP_HandleType;

typedef SCOREP_AnyHandle SCOREP_CallingContextHandle;

typedef SCOREP_AnyHandle SCOREP_CallpathHandle;

typedef SCOREP_AnyHandle SCOREP_GroupHandle;

typedef SCOREP_AnyHandle SCOREP_InterimCommunicatorHandle;

typedef SCOREP_AnyHandle SCOREP_InterruptGeneratorHandle;

typedef SCOREP_AnyHandle SCOREP_LocationHandle;

typedef SCOREP_AnyHandle SCOREP_LocationGroupHandle;

typedef SCOREP_AnyHandle SCOREP_LocationPropertyHandle;

typedef SCOREP_AnyHandle SCOREP_ParameterHandle;

typedef SCOREP_AnyHandle SCOREP_PropertyHandle;

typedef SCOREP_AnyHandle SCOREP_RmaWindowHandle;

typedef SCOREP_AnyHandle SCOREP_SamplingSetRecorderHandle;

typedef SCOREP_AnyHandle SCOREP_SourceCodeLocationHandle;

typedef SCOREP_AnyHandle SCOREP_StringHandle;

typedef SCOREP_AnyHandle SCOREP_SystemTreeNodeHandle;

typedef SCOREP_AnyHandle SCOREP_SystemTreeNodePropertyHandle;


#endif /* SCOREP_PUBLIC_HANDLES_H */
