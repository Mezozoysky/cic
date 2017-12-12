# FindAngelScript
# Module: locating AngelScript library with cmake
# Author: Stanislav Demyanovich <mezozoysky@gmail.com>
#
# Usage:
#	set( ENV{AngelScript_SDK} /path/to/sdk )
#	find_package( AngelScript REQUIRED )
#
# Defines:
# AngelScript_INCLUDE_DIR	path to AngelScript includes
# AngelScript_LIBRARY		path to AngelScript library file
# AngelScript_FOUND			true if AngelScript includes and library ware found
#
# Note:  Works with static library only. No time for more.

set( AngelScript_LOCATION_PREFIX_HINTS
    ${AngelScript_SDK}/angelscript
    $ENV{ANGELSCRIPT_SDK}/angelscript
    )

if( NOT AngelScript_LOCATION_PREFIX )

    find_path( AngelScript_LOCATION_PREFIX
        NAMES include/angelscript.h
        HINTS ${AngelScript_LOCATION_PREFIX_HINTS}
        DOC "AngelScript SDK directory, top dir. for AngelScript's 'include' and 'lib' dirs"
        )

    if( NOT AngelScript_LOCATION_PREFIX )
        if( AngelScript_FIND_REQUIRED )
            message( FATAL_ERROR "Cant find AngelScript location" )
        endif()
        if( NOT AngelScript_FIND_QUIETLY )
            message( STATUS "Cant find AngelScript location" )
        endif()
        return()
    else()
        message( STATUS "Found AngelScript location prefix: ${AngelScript_LOCATION_PREFIX}" )
    endif()

endif()

if ( NOT AngelScript_INCLUDE_DIR )
    set( AngelScript_INCLUDE_DIR "${AngelScript_LOCATION_PREFIX}/include" CACHE PATH "Path to AngelScript includes" )
    message( STATUS "Found AngelScript include dir: ${AngelScript_INCLUDE_DIR}" )
endif()

if( NOT AngelScript_INCLUDE_DIR )
    message( FATAL_ERROR "Cant find AngelScript includes" )
    return()
endif()

if( NOT AngelScript_LIBRARY )

    find_library( AngelScript_LIBRARY
        NAMES angelscript
        HINTS ${AngelScript_LOCATION_PREFIX}
        PATH_SUFFIXES lib
        )
    if( NOT AngelScript_LIBRARY )
        if( AngelScript_FIND_REQUIRED )
            message( FATAL_ERROR "Cant find AngelScript library" )
        endif()
        if( NOT AngelScript_FIND_QUIETLY )
            message( STATUS "Cant find AngelScript library" )
        endif()
    else()
        message( STATUS "Found AngelScript library: ${AngelScript_LIBRARY}" )
    endif()

    if( AngelScript_LIBRARY )
        set( AngelScript_FOUND TRUE )
    endif()

endif()
