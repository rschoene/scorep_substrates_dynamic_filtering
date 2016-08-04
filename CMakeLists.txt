cmake_minimum_required(VERSION 2.8)
project(dynamic_filtering_plugin)

option(BUILD_DEBUG "Include debugging symbols in library." OFF)
option(SCOREP_INCLUDE_DIRS "Path to Score-P sources." "")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/common)

#find_package(Scorep REQUIRED)

file(GLOB_RECURSE PLUGIN_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.h)

add_definitions("-Wall -pedantic -ansi -std=c99")

if(BUILD_DEBUG)
    add_definitions("-ggdb")
endif()

include_directories(${SCOREP_INCLUDE_DIRS}/include ${SCOREP_INCLUDE_DIRS}/src/measurement/substrates/include ${SCOREP_INCLUDE_DIRS}/src/measurement/definitions/include ${SCOREP_INCLUDE_DIRS}/src/measurement/include ${SCOREP_INCLUDE_DIRS}/vendor/common/src/utils/include)
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCE_FILES})
