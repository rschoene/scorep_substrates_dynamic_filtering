cmake_minimum_required(VERSION 2.8)
project(scorep_substrate_dynamic_filtering)

option(BUILD_DEBUG "Include debugging symbols in library and print some usefull output on execution." OFF)
set(HASH_FUNCTION "HASH_OWN" CACHE STRING "Use other than identity function as hash. See uthash docs for more info.")
set(MAX_THREAD_CNT "512" CACHE STRING "Change maximum number of observed threads.")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/common)

find_package(Scorep REQUIRED)

set(PLUGIN_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/dynamic-filtering.c)

add_definitions("-Wall -Wextra -pedantic -std=c11 -DHASH_FUNCTION=${HASH_FUNCTION} -DMAX_THREAD_CNT=${MAX_THREAD_CNT}")

if(BUILD_DEBUG)
	add_definitions("-ggdb -DDYNAMIC_FILTERING_DEBUG")
endif()

include_directories(${PROJECT_SOURCE_DIR}/src/ ${SCOREP_INCLUDE_DIRS})

add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCE_FILES})

find_library(UNW_LIB unwind HINT ${LIBUNWIND_ROOT}/lib)
find_path(UNW_INC libunwind.h HINT ${LIBUNWIND_ROOT}/include)
message(STATUS "libunwind.h found at ${UNW_INC}")
target_link_libraries(${PROJECT_NAME} "${UNW_LIB}")
target_include_directories(${PROJECT_NAME} PRIVATE "${UNW_INC}")
