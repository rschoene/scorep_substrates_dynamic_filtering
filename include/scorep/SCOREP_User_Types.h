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

#ifndef SCOREP_USER_TYPES_H
#define SCOREP_USER_TYPES_H

/**
   @file       SCOREP_User_Types.h
   @ingroup    SCOREP_User
   @brief      This file contains type definitions for manual user instrumentation.
 */

#include <scorep/SCOREP_PublicTypes.h>

/* **************************************************************************************
 * Typedefs
 * *************************************************************************************/

struct SCOREP_User_Region;

/**
   Type for region handles in the user adapter.
 */
typedef struct SCOREP_User_Region* SCOREP_User_RegionHandle;

/**
   @def SCOREP_USER_INVALID_REGION
   Value for uninitialized or invalid region handles
 */
#define SCOREP_USER_INVALID_REGION NULL

/**
   Type for the region type
 */
typedef uint32_t SCOREP_User_RegionType;

/**
   Type for the user metric type
 */
typedef uint32_t SCOREP_User_MetricType;

/**
   Type for parameter handles
 */
typedef uint64_t SCOREP_User_ParameterHandle;

/**
    @def SCOREP_USER_INVALID_PARAMETER
    Marks an parameter handle as invalid or uninitialized
 */
#define SCOREP_USER_INVALID_PARAMETER -1

/* **************************************************************************************
 * Defines for the Region types
 * *************************************************************************************/

/**
    @addtogroup SCOREP_User
    @{
 */

/**
    @name Region types
    @{
 */

/**
    @def SCOREP_USER_REGION_TYPE_COMMON
    Region without any specific type.
 */
#define SCOREP_USER_REGION_TYPE_COMMON 0

/**
    @def SCOREP_USER_REGION_TYPE_FUNCTION
    Marks the region as being the codeblock of a function.
 */
#define SCOREP_USER_REGION_TYPE_FUNCTION 1

/**
    @def SCOREP_USER_REGION_TYPE_LOOP
    Marks the region as being the codeblock of a loop with the same number of iterations
    on all processes.
 */
#define SCOREP_USER_REGION_TYPE_LOOP 2

/**
    @def SCOREP_USER_REGION_TYPE_DYNAMIC
    Marks the regions as dynamic.
 */
#define SCOREP_USER_REGION_TYPE_DYNAMIC  4

/**
    @def SCOREP_USER_REGION_TYPE_PHASE
    Marks the region as being a root node of a phase.
 */
#define SCOREP_USER_REGION_TYPE_PHASE    8

/** @} */

/* **************************************************************************************
 * Defines for the data type of a user counter
 * *************************************************************************************/

/**
    @name Metric types
    @{
 */

/**
    @def SCOREP_USER_METRIC_TYPE_INT64
    Indicates that a user counter is of data type signed 64 bit integer.
 */
#define SCOREP_USER_METRIC_TYPE_INT64 0

/**
    @def SCOREP_USER_METRIC_TYPE_UINT64
    Indicates that a user counter is of data type unsigned 64 bit integer.
 */
#define SCOREP_USER_METRIC_TYPE_UINT64 1

/**
    @def SCOREP_USER_METRIC_TYPE_DOUBLE
    Indicates that a user counter is of data type double.
 */
#define SCOREP_USER_METRIC_TYPE_DOUBLE 2

/**@}*/

/* **************************************************************************************
 * Defines for the context of user counters
 * *************************************************************************************/

/**
    @name Metric contexts
    @{
 */
/**
    @def SCOREP_USER_METRIC_CONTEXT_GLOBAL
    Indicates that a user counter is is measured for the global context.
 */
#define SCOREP_USER_METRIC_CONTEXT_GLOBAL 0

/**
    @def SCOREP_USER_METRIC_CONTEXT_CALLPATH
    Indicates that a user counter is is measured for every callpath.
 */
#define SCOREP_USER_METRIC_CONTEXT_CALLPATH 1

/**@}*/

/* **************************************************************************************
 * Default and uninitialized handles
 * *************************************************************************************/

/**
   @}
 */

#endif /* SCOREP_USER_TYPES_H */
