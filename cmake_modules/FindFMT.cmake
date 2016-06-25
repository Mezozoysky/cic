# FindFMT
# Module: locating FMT (formerly cppformat) library with cmake
# Author: Stanislav Demyanovich <mezozoysky@gmail.com>
#
# Usage:
#	set( ENV{FMT_DIR} /path/to/fmt )
#	find_package( FMT REQUIRED )
#
# Defines:
# FMT_INCLUDE_DIR	path to FMT includes
# FMT_LIBRARY		path to FMT library file
# FMT_FOUND			true if FMT includes and library ware found
#
# Note:  Works with static library only. No time for more.

set( FMT_LOCATION_PREFIX_HINTS
    ${FMT_DIR}
    $ENV{FMT_DIR}
    /usr/local
)

if( NOT FMT_LOCATION_PREFIX )
    find_path( FMT_LOCATION_PREFIX
        NAMES include/fmt/format.h
        HINTS ${FMT_LOCATION_PREFIX_HINTS}
        DOC "Top-level directory, usually its /usr/local"
    )
endif()

if( NOT FMT_LOCATION_PREFIX )
    if( FMT_FIND_REQUIRED )
        message( FATAL_ERROR "-- FindFMT -- cant find FMT location" )
    endif()
    if( NOT FMT_FIND_QUIETLY )
        message( STATUS "-- FindFMT -- cant find FMT location" )
    endif()
    return()
else()
    message( STATUS "-- FindFMT -- found FMT location prefix: ${FMT_LOCATION_PREFIX}" )
endif()

set( FMT_INCLUDE_DIR ${FMT_LOCATION_PREFIX}/include/ CACHE PATH "Path to FMT includes" )
message( STATUS "-- FindFMT -- found FMT include dir: ${FMT_INCLUDE_DIR}" )

find_library(
    FMT_LIBRARY
    NAMES fmt
    HINTS ${FMT_LOCATION_PREFIX}
    PATH_SUFFIXES
        lib
)
if( NOT FMT_LIBRARY )
    if( FMT_FIND_REQUIRED )
        message( FATAL_ERROR "-- FindFMT -- cant find FMT library" )
    endif()
    if( NOT FMT_FIND_QUIETLY )
        message( STATUS "-- FindFMT -- cant find FMT library" )
    endif()
else()
    message( STATUS "-- FindFMT -- found FMT library: ${FMT_LIBRARY}" )
endif()

if( FMT_LIBRARY )
    set( FMT_FOUND TRUE )
endif()
