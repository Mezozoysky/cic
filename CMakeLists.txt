cmake_minimum_required( VERSION 3.1.0 )

set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Project build type" )
set( CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Install prefix" )
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" )
set( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" )
set( CMAKE_VERBOSE_MAKEFILE TRUE )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake" )
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )

if( "$ENV{CATCH_HOME}" STREQUAL "" )
    set( ENV{CATCH_HOME} "${CMAKE_SOURCE_DIR}/deps/Catch/" )
endif()
set( Catch_HOME $ENV{CATCH_HOME} CACHE PATH "Path to Catch" )

if( "$ENV{POCO_HOME}" STREQUAL "" )
	set( ENV{POCO_HOME} "${CMAKE_SOURCE_DIR}/deps/Poco/" )
endif()
set( POCO_HOME $ENV{POCO_HOME} CACHE PATH "Top directory containing Poco library and includes" )
list( APPEND CMAKE_PREFIX_PATH "${POCO_HOME}" )

if( "$ENV{FMT_HOME}" STREQUAL "" )
    set( ENV{FMT_HOME} "${CMAKE_SOURCE_DIR}/deps/fmt" )
endif()
set( FMT_HOME $ENV{FMT_HOME} CACHE PATH "Top directory containing fmt library and includes" )
list( APPEND CMAKE_PREFIX_PATH "${FMT_HOME}" )


project( cic C CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQURED TRUE )

#
# OS definitions
#
message( "-- ${CMAKE_PROJECT_NAME} -- System name: ${CMAKE_SYSTEM_NAME}" )
message( "-- ${CMAKE_PROJECT_NAME} -- System version: ${CMAKE_SYSTEM_VERSION}" )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set( CIC_OS_LINUX TRUE )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set( CIC_OS_OSX TRUE )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( CIC_OS_WINDOWS TRUE )
else()
    message( FATAL_ERROR "UNSUPPORTED OS!" )
endif()

#
# Compiler options
#
message( "-- ${CMAKE_PROJECT_NAME} -- Compiler name: ${CMAKE_CXX_COMPILER_ID}" )
message( "-- ${CMAKE_PROJECT_NAME} -- Compiler version: ${CMAKE_CXX_COMPILER_VERSION}" )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    set( CIC_COMPILER_GCC TRUE )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" )
    set( CIC_COMPILER_CLANG TRUE )
else()
    #TODO: investigate what to do on windows
endif()

#
# Global dependencies
#
set( Catch_USE_SINGLE_INCLUDE TRUE )
find_package( Catch REQUIRED )
find_package( Poco 1.7.8 CONFIG REQUIRED Util XML JSON Foundation )
find_package( fmt 4.0.0 CONFIG REQUIRED )
find_package( Threads REQUIRED )

#
# Modules' build options
#
option( CIC_BUILD_PRFRM_APP "Build prfrm tool"  ON )
option( CIC_BUILD_PLAN_LIB "Build cic-plan library"  ON )
option( CIC_BUILD_INDUSTRY_LIB "Build cic-industry library"  ON )
option( CIC_BUILD_SCRIPTING_LIB "Build cic-scripting library"  ON )

#
# Modules
#
if ( CIC_BUILD_SCRIPTING_LIB OR CIC_BUILD_PRFRM_APP )
    add_subdirectory( deps/angelscript/ )
endif()

add_subdirectory( src )
