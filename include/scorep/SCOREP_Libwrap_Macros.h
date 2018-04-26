/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2013-2014, 2017,
 * Technische Universitaet Dresden, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */

/**
 * @brief These macros represent the interface for library wrapping
 *        to the Score-P measurement core.
 *
 * @file
 */

#ifndef SCOREP_LIBWRAP_MACROS_H
#define SCOREP_LIBWRAP_MACROS_H

#include <stdlib.h>
#include <scorep/SCOREP_Libwrap.h>


/** @def SCOREP_LIBWRAP_NULL
 *  NULL handle within Score-P library wrapper
 */
#define SCOREP_LIBWRAP_NULL NULL

#define SCOREP_LIBWRAP_INIT( handle, attributes )                   \
    do                                                              \
    {                                                               \
        if ( ( handle ) == SCOREP_LIBWRAP_NULL )                    \
        {                                                           \
            SCOREP_Libwrap_Create( &( handle ), &( attributes ) );  \
        }                                                           \
    }                                                               \
    while ( 0 )

#ifndef SCOREP_LIBWRAP_REGION_HANDLE

/**
 * @def SCOREP_LIBWRAP_REGION_HANDLE
 * The name of the SCOREP_RegionHandle for the function @p func.
 *
 * @param func Function name
 */
#define SCOREP_LIBWRAP_REGION_HANDLE( func ) \
    scorep_libwrap_region__ ## func

#endif

#ifndef SCOREP_LIBWRAP_REGION_FILTERED

/**
 * @def SCOREP_LIBWRAP_REGION_FILTERED
 * The name of the filtered predicate for the function @p func.
 *
 * @param func Function name
 */
#define SCOREP_LIBWRAP_REGION_FILTERED( func ) \
    scorep_libwrap_region_filtered__ ## func

#endif

/**
 * @def SCOREP_LIBWRAP_ENTER_MEASUREMENT
 * Entering the wrapper function
 */
#define SCOREP_LIBWRAP_ENTER_MEASUREMENT() \
    int scorep_libwrap_var_previous; \
    int scorep_libwrap_var_trigger = SCOREP_Libwrap_EnterMeasurement()

/**
 * @def SCOREP_LIBWRAP_EXIT_MEASUREMENT
 * Exiting the wrapper function
 */
#define SCOREP_LIBWRAP_EXIT_MEASUREMENT() \
    SCOREP_Libwrap_ExitMeasurement()

/**
 * @def SCOREP_LIBWRAP_ENTER_WRAPPED_REGION
 *      Transition from wrapper to wrappee
 */
#define SCOREP_LIBWRAP_ENTER_WRAPPED_REGION() \
    scorep_libwrap_var_previous = SCOREP_Libwrap_EnterWrappedRegion()

/**
 * @def SCOREP_LIBWRAP_EXIT_WRAPPED_REGION
 *      Transition from wrappee to wrapper
 */
#define SCOREP_LIBWRAP_EXIT_WRAPPED_REGION() \
    SCOREP_Libwrap_ExitWrappedRegion( scorep_libwrap_var_previous )

/**
 * @def SCOREP_LIBWRAP_FUNC_ENTER
 * Write event for entering wrapped function
 *
 */
#define SCOREP_LIBWRAP_FUNC_ENTER( func )                                               \
    do                                                                                  \
    {                                                                                   \
        if ( scorep_libwrap_var_trigger )                                               \
        {                                                                               \
            if ( !SCOREP_LIBWRAP_REGION_FILTERED( func ) )                              \
            {                                                                           \
                SCOREP_Libwrap_EnterRegion( SCOREP_LIBWRAP_REGION_HANDLE( func ) );     \
            }                                                                           \
            else                                                                        \
            {                                                                           \
                SCOREP_Libwrap_EnterWrapper( SCOREP_LIBWRAP_REGION_HANDLE( func ) );    \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
    while ( 0 )

/**
 * @def SCOREP_LIBWRAP_FUNC_EXIT
 * Write event for leaving wrapped function
 *
 */
#define SCOREP_LIBWRAP_FUNC_EXIT( func )                                            \
    do                                                                              \
    {                                                                               \
        if ( scorep_libwrap_var_trigger )                                           \
        {                                                                           \
            if ( !SCOREP_LIBWRAP_REGION_FILTERED( func ) )                          \
            {                                                                       \
                SCOREP_Libwrap_ExitRegion( SCOREP_LIBWRAP_REGION_HANDLE( func ) );  \
            }                                                                       \
            else                                                                    \
            {                                                                       \
                SCOREP_Libwrap_ExitWrapper( SCOREP_LIBWRAP_REGION_HANDLE( func ) ); \
            }                                                                       \
        }                                                                           \
    }                                                                               \
    while ( 0 )

/** @internal
 *  @def _SCOREP_LIBWRAP_RETTYPE
 *
 *  Resolves the parentheses of the provided rettype of SCOREP_LIBWRAP_DECLARE_REAL_FUNC
 */
#define _SCOREP_LIBWRAP_RETTYPE( ... ) __VA_ARGS__

/* ********************************************************************
* STATIC MODE
* ********************************************************************/
#if defined( SCOREP_LIBWRAP_STATIC )

/**
 * @def SCOREP_LIBWRAP_MODE
 * Define static library wrapper mode
 */
#define SCOREP_LIBWRAP_MODE SCOREP_LIBWRAP_MODE_STATIC

/**
 * @def SCOREP_LIBWRAP_FUNC_NAME
 * Macro for parameter expansion
 *
 * @param func              Function name
 */
#define SCOREP_LIBWRAP_FUNC_NAME( func ) __wrap_##func

#ifndef SCOREP_LIBWRAP_FUNC_REAL_NAME

/**
 * @def SCOREP_LIBWRAP_FUNC_REAL_NAME
 * The symbol name of the original function. For static wrapping, this defaults
 * to the `__real_`-symbol.
 *
 * @param func              Function name
 */
#define SCOREP_LIBWRAP_FUNC_REAL_NAME( func ) __real_##func

#endif

/**
 * @def SCOREP_LIBWRAP_DECLARE_REAL_FUNC
 * Declares the symbol name of the original function. For static wrapping, this defaults
 * to the `__real_`-symbol.
 *
 * @param rettype           Function return type in parantheses.
 * @param func              Function name
 * @param argtypes          Function arguments in parantheses
 */
#define SCOREP_LIBWRAP_DECLARE_REAL_FUNC( rettype, func, argtypes ) \
    _SCOREP_LIBWRAP_RETTYPE rettype SCOREP_LIBWRAP_FUNC_REAL_NAME( func )argtypes

/**
 * @def SCOREP_LIBWRAP_DEFINE_REAL_FUNC
 * Defines the symbol name of the original function. For static wrapping,
 * this is not needed, thus empty.
 *
 * @param rettype           Function return type in parantheses.
 * @param func              Function name
 * @param argtypes          Function arguments in parantheses
 */
#define SCOREP_LIBWRAP_DEFINE_REAL_FUNC( rettype, func, argtypes )

/**
 * @def SCOREP_LIBWRAP_FUNC_CALL
 * Call real function symbol
 *
 * @param func             Function name
 * @param args             Function arguments
 */
#define SCOREP_LIBWRAP_FUNC_CALL( func, args ) \
    SCOREP_LIBWRAP_FUNC_REAL_NAME( func )args

/**
 * @def SCOREP_LIBWRAP_FUNC_INIT
 * Prepare wrapping of requested function
 *
 * @param handle            Library wrapper handle
 * @param func              Function symbol
 * @param prettyname        The pretty name of the function (i.e., demangled; as `const char*`)
 * @param file              Source code location (file as `const char*`)
 * @param line              Source code location (line as `int`)
 */
#define SCOREP_LIBWRAP_FUNC_INIT( handle, func, prettyname, file, line )        \
    do                                                                          \
    {                                                                           \
        SCOREP_Libwrap_DefineRegion( handle,                                    \
                                     &SCOREP_LIBWRAP_REGION_HANDLE( func ),     \
                                     &SCOREP_LIBWRAP_REGION_FILTERED( func ),   \
                                     prettyname, #func, file, line );           \
    }                                                                           \
    while ( 0 )

/* *********************************************************************
 * end of static mode
 * ********************************************************************/


/* *********************************************************************
 *  SHARED MODE
 * ********************************************************************/
#elif defined( SCOREP_LIBWRAP_SHARED )

/** @def SCOREP_LIBWRAP_MODE
 *  Define shared library wrapper mode
 */
#define SCOREP_LIBWRAP_MODE SCOREP_LIBWRAP_MODE_SHARED

/**
 * @def SCOREP_LIBWRAP_FUNC_NAME
 * Macro for parameter expansion
 *
 * @param func             Function name
 */
#define SCOREP_LIBWRAP_FUNC_NAME( func ) func

#ifndef SCOREP_LIBWRAP_FUNC_REAL_NAME

/**
 * @def SCOREP_LIBWRAP_FUNC_REAL_NAME
 * The symbol name of the original function. For shared wrapping, this should be
 * the symbol of the function pointer.
 *
 * @param func              Function name
 */
#define SCOREP_LIBWRAP_FUNC_REAL_NAME( func ) \
    scorep_libwrap_funcptr__ ## func

#endif

/**
 * @def SCOREP_LIBWRAP_FUNC_TYPE
 * Type of the functionDefines the symbol name of the original function. For runtime wrapping,
 * this defines the function pointer variable and initializes with NULL
 *
 * @param rettype           Function return type in parantheses.
 * @param name              A name
 * @param argtypes          Function arguments in parantheses
 */
#define SCOREP_LIBWRAP_FUNC_TYPE( rettype, name, argtypes ) \
    _SCOREP_LIBWRAP_RETTYPE rettype( *name ) argtypes

/**
 * @def SCOREP_LIBWRAP_DECLARE_REAL_FUNC_SPECIFIER
 * The defualt storage specifier for the real-func declaration is 'extern'.
 * Overwrite this macro if you want a different.
 */
#ifndef SCOREP_LIBWRAP_DECLARE_REAL_FUNC_SPECIFIER

#define SCOREP_LIBWRAP_DECLARE_REAL_FUNC_SPECIFIER extern

#endif

/**
 * @def SCOREP_LIBWRAP_DECLARE_REAL_FUNC
 * Declares the symbol name of the original function. For runtime wrapping,
 * this declares the function pointer as extern.
 *
 * @param rettype           Function return type in parantheses.
 * @param func              Function name
 * @param argtypes          Function arguments in parantheses
 */
#define SCOREP_LIBWRAP_DECLARE_REAL_FUNC( rettype, func, argtypes ) \
    SCOREP_LIBWRAP_DECLARE_REAL_FUNC_SPECIFIER SCOREP_LIBWRAP_FUNC_TYPE( rettype, SCOREP_LIBWRAP_FUNC_REAL_NAME( func ), argtypes )

/**
 * @def SCOREP_LIBWRAP_DEFINE_REAL_FUNC
 * Defines the symbol name of the original function. For runtime wrapping,
 * this defines the function pointer variable and initializes with NULL
 *
 * @param rettype           Function return type in parantheses.
 * @param func              Function name
 * @param argtypes          Function arguments in parantheses
 */
#define SCOREP_LIBWRAP_DEFINE_REAL_FUNC( rettype, func, argtypes ) \
    SCOREP_LIBWRAP_FUNC_TYPE( rettype, SCOREP_LIBWRAP_FUNC_REAL_NAME( func ), argtypes ) = NULL

/**
 * @def SCOREP_LIBWRAP_FUNC_CALL
 * Call real function symbol
 *
 * @param func              Function name
 * @param args              Function arguments
 */
#define SCOREP_LIBWRAP_FUNC_CALL( func, args ) \
    SCOREP_LIBWRAP_FUNC_REAL_NAME( func )args

/**
 * @def SCOREP_LIBWRAP_FUNC_INIT
 * Prepare wrapping of requested function
 *
 * @param handle            Library wrapper handle
 * @param func              Function symbol
 * @param prettyname        The pretty name of the function (i.e., demangled; as `const char*`)
 * @param file              Source code location (file as `const char*`)
 * @param line              Source code location (line as `int`)
 */
#define SCOREP_LIBWRAP_FUNC_INIT( handle, func, prettyname, file, line )                        \
    do                                                                                          \
    {                                                                                           \
        SCOREP_Libwrap_SharedPtrInit( handle, #func,                                            \
                                      ( void** )( &SCOREP_LIBWRAP_FUNC_REAL_NAME( func ) ) );   \
        SCOREP_Libwrap_DefineRegion( handle,                                                    \
                                     &SCOREP_LIBWRAP_REGION_HANDLE( func ),                     \
                                     &SCOREP_LIBWRAP_REGION_FILTERED( func ),                   \
                                     prettyname, #func, file, line );                           \
    }                                                                                           \
    while ( 0 )

/* *********************************************************************
 * END OF SHARED MODE
 * ********************************************************************/


/* *********************************************************************
 * WEAK SYMBOL MODE
 * ********************************************************************/
#elif defined( SCOREP_LIBWRAP_WEAK )

/** @def SCOREP_LIBWRAP_MODE
 *  Define weak symbol library wrapper mode
 */
#define SCOREP_LIBWRAP_MODE SCOREP_LIBWRAP_MODE_WEAK

/**
 * @def SCOREP_LIBWRAP_FUNC_NAME
 * Macro for parameter expansion
 *
 * @param func              Function name
 */
#define SCOREP_LIBWRAP_FUNC_NAME( func ) func

#ifndef SCOREP_LIBWRAP_FUNC_REAL_NAME

/**
 * @def SCOREP_LIBWRAP_FUNC_REAL_NAME
 * The symbol name of the original function. For weak wrapping, this should
 * point to the strong symbol and defaults to the 'p'-symbol.
 *
 * @param func              Function name
 */
#define SCOREP_LIBWRAP_FUNC_REAL_NAME( func ) p##func

#endif

/**
 * @def SCOREP_LIBWRAP_FUNC_CALL
 * Call real function symbol
 *
 * @param func              Function name
 * @param args              Function arguments
 */
#define SCOREP_LIBWRAP_FUNC_CALL( func, args ) \
    SCOREP_LIBWRAP_FUNC_REAL_NAME( func )args

/**
 * @def SCOREP_LIBWRAP_DECLARE_REAL_FUNC
 * Declares the symbol name of the original function. For weak/strong wrapping,
 * this declares the strong function.
 *
 * @param rettype           Function return type in parantheses.
 * @param func              Function name
 * @param argtypes          Function arguments in parantheses
 */
#define SCOREP_LIBWRAP_DECLARE_REAL_FUNC( rettype, func, argtypes ) \
    _SCOREP_LIBWRAP_RETTYPE rettype SCOREP_LIBWRAP_FUNC_REAL_NAME( func )argtypes

/**
 * @def SCOREP_LIBWRAP_DEFINE_REAL_FUNC
 * Defines the symbol name of the original function. For weak/strong wrapping,
 * this is not needed, thus empty.
 *
 * @param rettype           Function return type in parantheses.
 * @param func              Function name
 * @param argtypes          Function arguments in parantheses
 */
#define SCOREP_LIBWRAP_DEFINE_REAL_FUNC( rettype, func, argtypes )

/**
 * @def SCOREP_LIBWRAP_FUNC_INIT
 * Prepare wrapping of requested function
 *
 * @param handle            Library wrapper handle
 * @param func              Function symbol
 * @param prettyname        The pretty name of the function (i.e., demangled; as `const char*`)
 * @param file              Source code location (file as `const char*`)
 * @param line              Source code location (line as `int`)
 */
#define SCOREP_LIBWRAP_FUNC_INIT( handle, func, prettyname, file, line )        \
    do                                                                          \
    {                                                                           \
        SCOREP_Libwrap_DefineRegion( handle,                                    \
                                     &SCOREP_LIBWRAP_REGION_HANDLE( func ),     \
                                     &SCOREP_LIBWRAP_REGION_FILTERED( func ),   \
                                     prettyname, #func, file, line );           \
    }                                                                           \
    while ( 0 )

/* *********************************************************************
 * END OF WEAK MODE
 * ********************************************************************/

#else

#error no library wrapping mode specified

#endif /* link modes */

#endif /* SCOREP_LIBWRAP_MACROS_H */
