/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2013-2015, 2017,
 * Technische Universitaet Dresden, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */

/**
 * @brief Definitions of library wrapping functions
 *        which expands from macros
 *
 * @file
 */

#ifndef SCOREP_LIBWRAP_H
#define SCOREP_LIBWRAP_H

#include <stdint.h>
#include <scorep/SCOREP_PublicTypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** Version of this library API/ABI */
#define SCOREP_LIBWRAP_VERSION 1

/** @brief Keeps all necessary information of the library wrapping object. */
typedef struct SCOREP_LibwrapHandle SCOREP_LibwrapHandle;

/** Determine which mode of library type is used */
typedef enum SCOREP_LibwrapMode
{
    SCOREP_LIBWRAP_MODE_SHARED,  /**< wrapping dynamically linked library */
    SCOREP_LIBWRAP_MODE_STATIC,  /**< wrapping statically linked library */
    SCOREP_LIBWRAP_MODE_WEAK,    /**< wrapping a library that provides weak symbols */
} SCOREP_LibwrapMode;

/** Data structure for library wrapper attributes */
typedef struct SCOREP_LibwrapAttributes SCOREP_LibwrapAttributes;
struct SCOREP_LibwrapAttributes
{
    int                version;
    const char*        name;
    const char*        display_name;
    SCOREP_LibwrapMode mode;
    void               ( * init )( SCOREP_LibwrapHandle* libwrapHandle );
    int                number_of_shared_libs;
    const char**       shared_libs;
};


/* function definitions */

/**
 * This function calls the internal Score-P function to create a region
 * handle and will return a valid region handle for the function.
 *
 * @param handle            Score-P library wrapper object
 * @param region            Score-P region handle
 * @param regionFiltered    Predicate, whether region was filtered
 * @param name              Region name
 * @param symbol            Symbol name (i.e, demangled)
 * @param file              Source file name
 * @param line              Line number in source file
 */
extern void
SCOREP_Libwrap_DefineRegion( SCOREP_LibwrapHandle* handle,
                             SCOREP_RegionHandle*  region,
                             int*                  regionFiltered,
                             const char*           name,
                             const char*           symbol,
                             const char*           file,
                             int                   line );

/**
 * This function opens the shared libraries and fill the handle array of
 * the wrapper object with the associated library handles. In all modes
 * it will build the wrapper object and fill the attributes structure.
 *
 * @param handle            Score-P library wrapper object
 * @param attributes        Attributes of the Score-P library wrapper object
 */
extern void
SCOREP_Libwrap_Create( SCOREP_LibwrapHandle**          handle,
                       const SCOREP_LibwrapAttributes* attributes );

/**
 * Call this function only if you want to wrap a shared library. It
 * extracts the function pointers out the library handles and returns
 * a function pointer to the actual symbol.
 *
 * @param handle            Score-P library wrapper object
 * @param func              Name of wrapped function
 * @param funcPtr           Pointer to the actual symbol
 */
extern void
SCOREP_Libwrap_SharedPtrInit( SCOREP_LibwrapHandle* handle,
                              const char*           func,
                              void**                funcPtr );

/**
 * Early initialize function pointers for runtime wrapping. Can already be
 * called outside of the measurement.
 *
 * @param func     Name of wrapped function
 * @param funcPtr  Pointer to the actual symbol
 */
extern void
SCOREP_Libwrap_EarlySharedPtrInit( const char* func,
                                   void**      funcPtr );

/**
 * Enter the measurement. First action a wrapper must do.
 *
 * @return Returns true if the wrapper is allowed to call any other measurement
 *         functions (except @a SCOREP_Libwrap_ExitMeasurement).
 */
extern int
SCOREP_Libwrap_EnterMeasurement( void );

/**
 * Exit the measurement. Last action (before any return statement) a wrapper must do.
 */
extern void
SCOREP_Libwrap_ExitMeasurement( void );

/**
 * Wrapper to write an event for entering wrapped function.
 *
 * @param region            Handle of entered region
 */
extern void
SCOREP_Libwrap_EnterRegion( SCOREP_RegionHandle region );

/**
 * Wrapper to write an event for leaving wrapped function.
 *
 * @param region            Handle of entered region
 */
extern void
SCOREP_Libwrap_ExitRegion( SCOREP_RegionHandle region );

/**
 * Enter the wrapper. Needs to be called if no @a SCOREP_Libwrap_EnterRegion
 * was called (e.g., because of filtering)
 *
 * @param region            Handle of entered region
 */
extern void
SCOREP_Libwrap_EnterWrapper( SCOREP_RegionHandle region );

/**
 * Exit the wrapper. Needs to be called if no @a SCOREP_Libwrap_ExitRegion
 * was called (e.g., because of filtering)
 *
 * @param region            Handle of entered region
 */
extern void
SCOREP_Libwrap_ExitWrapper( SCOREP_RegionHandle region );

/**
 * Marks the transition between wrapper and wrappee. I.e., just before the wrapper
 * calls the wrappee.
 *
 * @return previous state, must be passed on to @a SCOREP_Libwrap_ExitWrappedRegion.
 */
extern int
SCOREP_Libwrap_EnterWrappedRegion( void );

/**
 * Marks the transition between wrapper and wrappee. I.e., just before the wrapper
 * calls the wrappee.
 *
 * @param previous The return value from @a SCOREP_Libwrap_EnterWrappedRegion.
 */
extern void
SCOREP_Libwrap_ExitWrappedRegion( int previous );

#ifdef __cplusplus
};
#endif

#endif /* SCOREP_LIBWRAP_H */
