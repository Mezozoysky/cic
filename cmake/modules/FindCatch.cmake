# FindCatch
# Module: locating Catch library with cmake
# Author: Stanislav Demyanovich <mezozoysky@gmail.com>
#
# Usage:
#	set( ENV{Catch_HOME} /path/to/Catch )
#   set( Catch_USE_SINGLE_INCLUDE TRUE ) # TRUE for using single-include release
#	find_package( Catch REQUIRED )
#
# Defines:
# Catch_INCLUDE_DIR	   path to Catch includes or single include
# Catch_FOUND		   true if susa includes or single include were found

set( Catch_LOCATION_PREFIX_HINTS
    "${Catch_HOME}"
	"$ENV{CATCH_HOME}"
)

set( Catch_FIND_NAMES )
if( Catch_USE_SINGLE_INCLUDE)
    set( Catch_FIND_NAMES "single_include/catch.hpp" )
else()
    set( Catch_FIND_NAMES "include/catch.hpp" )
endif()

if( NOT Catch_LOCATION_PREFIX )
    find_path( Catch_LOCATION_PREFIX
        NAMES ${Catch_FIND_NAMES}
        HINTS ${Catch_LOCATION_PREFIX_HINTS}
        DOC "Top-level directory"
    )
endif()

if( NOT Catch_LOCATION_PREFIX )
    if( Catch_FIND_REQUIRED )
        message( STATUS "HINTS: ${Catch_LOCATION_PREFIX_HINTS}" )
        message( STATUS "NAMES: ${Catch_FIND_NAMES}" )
        message( STATUS "PREFIX: ${Catch_LOCATION_PREFIX}" )
        message( FATAL_ERROR "Cant find Catch location" )
    endif()
    if( NOT Catch_FIND_QUIETLY )
        message( STATUS "Cant find Catch location" )
    endif()
    return()
else()
    message( STATUS "Found Catch location prefix: ${Catch_LOCATION_PREFIX}" )
endif()

if( Catch_USE_SINGLE_INCLUDE )
    set( Catch_INCLUDE_DIR ${Catch_LOCATION_PREFIX}/single_include CACHE PATH "Path to Catch single include" )
    message( STATUS "Found Catch single include dir: ${Catch_INCLUDE_DIR}" )
else()
    set( Catch_INCLUDE_DIR ${Catch_LOCATION_PREFIX}/include CACHE PATH "Path to Catch includes" )
    message( STATUS "Found Catch include dir: ${Catch_INCLUDE_DIR}" )
endif()

if( Catch_INCLUDE_DIR )
    set( Catch_FOUND TRUE )
endif()
