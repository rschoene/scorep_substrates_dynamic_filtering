cmake_minimum_required(VERSION 2.8)
project(dynamic_filtering_plugin)

option(BUILD_DEBUG "Include debugging symbols in library and print some usefull output on execution." OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/common)

find_package(Scorep REQUIRED)

set(PLUGIN_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/dynamic-filtering.c)

add_definitions("-Wall -Wextra -pedantic -std=c11")

if(BUILD_DEBUG)
	add_definitions("-ggdb -DDYNAMIC_FILTERING_DEBUG")
endif()

include_directories(${PROJECT_SOURCE_DIR}/src/ ${SCOREP_INCLUDE_DIRS})
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCE_FILES})
