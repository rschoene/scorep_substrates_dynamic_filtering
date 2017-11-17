!
!  This file is part of the Score-P software (http://www.score-p.org)
!
!  Copyright (c) 2009-2011,
!  RWTH Aachen University, Germany
!
!  Copyright (c) 2009-2011,
!  Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
!
!  Copyright (c) 2009-2011, 2014-2015,
!  Technische Universitaet Dresden, Germany
!
!  Copyright (c) 2009-2011,
!  University of Oregon, Eugene, USA
!
!  Copyright (c) 2009-2011, 2013,
!  Forschungszentrum Juelich GmbH, Germany
!
!  Copyright (c) 2009-2011,
!  German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
!
!  Copyright (c) 2009-2011, 2015-2016,
!  Technische Universitaet Muenchen, Germany
!
!  Copyright (c) 2016,
!  Technische Universitaet Darmstadt, Germany
!
!  This software may be modified and distributed under the terms of
!  a BSD-style license.  See the COPYING file in the package base
!  directory for details.
!

!> @file
!! @ingroup    SCOREP_User_Interface
!!
!! @brief User interface for manual instrumentation of Fortran files.
!<

#ifdef SCOREP_USER_ENABLE

#ifdef __PGI
#define SCOREP_USER_DECL_VAR( type, variable, value ) type variable
#else
#define SCOREP_USER_DECL_VAR( type, variable, value ) type, save :: variable = value
#endif

!************************************************************* Data types

#define SCOREP_USER_REGION_HANDLE    integer*8
#define SCOREP_USER_METRIC_HANDLE    integer*8
#define SCOREP_USER_PARAMETER_HANDLE integer*8

!************************************************************* Constants

#define SCOREP_USER_INVALID_REGION    -1
#define SCOREP_USER_INVALID_METRIC    -1
#define SCOREP_USER_INVALID_PARAMETER -1

#define SCOREP_USER_REGION_TYPE_COMMON    0
#define SCOREP_USER_REGION_TYPE_FUNCTION  1
#define SCOREP_USER_REGION_TYPE_LOOP      2
#define SCOREP_USER_REGION_TYPE_DYNAMIC   4
#define SCOREP_USER_REGION_TYPE_PHASE     8

#define SCOREP_USER_METRIC_TYPE_INT64  0
#define SCOREP_USER_METRIC_TYPE_UINT64 1
#define SCOREP_USER_METRIC_TYPE_DOUBLE 2

#define SCOREP_USER_METRIC_CONTEXT_GLOBAL   0
#define SCOREP_USER_METRIC_CONTEXT_CALLPATH 1

!************************************************************* Regions

#define SCOREP_USER_REGION_DEFINE( handle ) \
        SCOREP_USER_DECL_VAR( SCOREP_USER_REGION_HANDLE, handle, SCOREP_USER_INVALID_REGION )

#define SCOREP_USER_REWIND_DEFINE( handle ) \
        SCOREP_USER_DECL_VAR( SCOREP_USER_REGION_HANDLE, handle, SCOREP_USER_INVALID_REGION )

#define SCOREP_USER_OA_PHASE_BEGIN( handle, name, type )\
        call SCOREP_F_OaBegin( handle,name,type,"",__LINE__)

#define SCOREP_USER_OA_PHASE_END( handle ) \
        call SCOREP_F_OaEnd( handle )

#define SCOREP_USER_REGION_BEGIN( handle, name, type )\
        call SCOREP_F_Begin(handle,name,type,"",__LINE__)

#define SCOREP_USER_REGION_BY_NAME_BEGIN( name, type )\
        call SCOREP_F_RegionByNameBegin(name,type,"",0)

#define SCOREP_USER_REWIND_POINT( handle, name ) \
        call SCOREP_F_RewindBegin(handle,name,SCOREP_USER_REGION_TYPE_COMMON,"",__LINE__)

#define SCOREP_USER_REGION_INIT( handle, name, type )\
        call SCOREP_F_Init(handle,name,type,"",__LINE__)

#define SCOREP_USER_REGION_END( handle ) call SCOREP_F_RegionEnd( handle )

#define SCOREP_USER_REGION_BY_NAME_END( name ) call SCOREP_F_RegionByNameEnd(name)

#define SCOREP_USER_REWIND_CHECK( handle, value ) call SCOREP_F_RewindRegionEnd( handle, value )

#define SCOREP_USER_REGION_ENTER( handle ) call SCOREP_F_RegionEnter( handle )

#define SCOREP_USER_FUNC_DEFINE() SCOREP_USER_REGION_DEFINE( scrpfh )

#define SCOREP_USER_FUNC_BEGIN( name ) \
        SCOREP_USER_REGION_BEGIN(scrpfh, name, \
        SCOREP_USER_REGION_TYPE_FUNCTION)

#define SCOREP_USER_FUNC_END() SCOREP_USER_REGION_END(scrpfh)

!************************************************************* Parameter

#define SCOREP_USER_PARAMETER_DEFINE( handle ) \
        SCOREP_USER_DECL_VAR( SCOREP_USER_PARAMETER_HANDLE, handle, SCOREP_USER_INVALID_PARAMETER )

#define SCOREP_USER_PARAMETER_INT64( handle, name, value ) \
        call SCOREP_F_ParameterInt64( handle, name, value )

#define SCOREP_USER_PARAMETER_UINT64( handle, name, value ) \
        call SCOREP_F_ParameterUint64( handle, name, value )

#define SCOREP_USER_PARAMETER_STRING( handle, name, value ) \
        call SCOREP_F_ParameterString( handle, name, value )

!************************************************************* User metric

#define SCOREP_USER_METRIC_LOCAL( metricHandle ) \
        SCOREP_USER_DECL_VAR( SCOREP_USER_METRIC_HANDLE, metricHandle, SCOREP_USER_INVALID_METRIC)

#define SCOREP_USER_METRIC_INIT( metricHandle, name, unit, type, context ) \
        call SCOREP_F_InitMetric(metricHandle,name,unit,type,context)

#define SCOREP_USER_METRIC_INT64( metricHandle, value ) \
        call SCOREP_F_MetricInt64( metricHandle, value )

#define SCOREP_USER_METRIC_UINT64( metricHandle, value ) \
        call SCOREP_F_MetricUint64( metricHandle, value )

#define SCOREP_USER_METRIC_DOUBLE( metricHandle, value ) \
        call SCOREP_F_MetricDouble( metricHandle, value )

!************************************************************* User control

#define SCOREP_RECORDING_ON() call SCOREP_F_EnableRecording()

#define SCOREP_RECORDING_OFF() call SCOREP_F_DisableRecording()

#define SCOREP_RECORDING_IS_ON( isOn ) call SCOREP_F_RecordingEnabled( isOn )

!************************************************************* Empty defines

#else

#define SCOREP_USER_REGION_DEFINE( handle )
#define SCOREP_USER_REWIND_DEFINE( handle )
#define SCOREP_USER_OA_PHASE_BEGIN( handle, name, type )
#define SCOREP_USER_OA_PHASE_END( handle )
#define SCOREP_USER_REWIND_POINT( handle, name )
#define SCOREP_USER_REGION_BEGIN( handle, name, type )
#define SCOREP_USER_REGION_INIT( handle, name, type )
#define SCOREP_USER_REGION_END( handle )
#define SCOREP_USER_REGION_BY_NAME_BEGIN( name, type )
#define SCOREP_USER_REGION_BY_NAME_END( name )
#define SCOREP_USER_REWIND_CHECK( handle, value )
#define SCOREP_USER_REGION_ENTER( handle )
#define SCOREP_USER_FUNC_DEFINE()
#define SCOREP_USER_FUNC_BEGIN( name )
#define SCOREP_USER_FUNC_END()
#define SCOREP_USER_GLOBAL_REGION_DEFINE( handle )
#define SCOREP_USER_GLOBAL_REGION_EXTERNAL( handle )
#define SCOREP_USER_PARAMETER_DEFINE( handle )
#define SCOREP_USER_PARAMETER_INT64( handle, name, value )
#define SCOREP_USER_PARAMETER_UINT64( handle, name, value )
#define SCOREP_USER_PARAMETER_STRING( handle, name, value )
#define SCOREP_USER_METRIC_GLOBAL( metricHandle )
#define SCOREP_USER_METRIC_EXTERNAL( metricHandle )
#define SCOREP_USER_METRIC_LOCAL( metricHandle )
#define SCOREP_USER_METRIC_INIT( metricHandle, name, unit, type, context )
#define SCOREP_USER_METRIC_INT64( metricHandle, value )
#define SCOREP_USER_METRIC_UINT64( metricHandle, value )
#define SCOREP_USER_METRIC_DOUBLE( metricHandle, value )
#define SCOREP_RECORDING_ON()
#define SCOREP_RECORDING_OFF()
#define SCOREP_RECORDING_IS_ON( isOn ) isOn = .false.

#endif
