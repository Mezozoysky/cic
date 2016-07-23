cmake_minimum_required( VERSION 3.0 )

set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Project build type" )
set( CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Install prefix" )
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH} )
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" )
set( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" )
set( CMAKE_MACOSX_RPATH 1 )
set( CMAKE_VERBOSE_MAKEFILE 1 )

project( CICheck C CXX )

option( CIC_BUILD_INDUSTRY_LIB "Build cic-industry library"  ON )
option( CIC_BUILD_XMLU_LIB "Build cic-xmlu library"  ON )
option( CIC_BUILD_TASK_LIB "Build cic-task library"  ON )
option( CIC_BUILD_GOAL_LIB "Build cic-goal library"  ON )
option( CIC_BUILD_XGOAL_LIB "Build cic-xgoal library"  ON )
option( CIC_BUILD_CICHECK "Build cicheck tool"  ON )
option( CIC_BUILD_CHECK "Build cic-check tool"  ON )

set( Poco_DIR "${CMAKE_SOURCE_DIR}/deps/" CACHE PATH "Directory containing Poco libraries and includes" )
set( FMT_DIR "${CMAKE_SOURCE_DIR}/deps/" CACHE PATH "Deractory containing FMT library and includes" )

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
    set( CMAKE_CXX_FLAGS "-std=c++11" )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" )
    set( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
else()
    #TODO: investigate what to do on windows
endif()

#
# Enable project folders
#
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake" )


add_subdirectory( src )
