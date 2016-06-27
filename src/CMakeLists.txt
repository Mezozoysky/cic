find_package( Poco REQUIRED Foundation XML JSON Util )
if( Poco_FOUND )
	include_directories( ${Poco_INCLUDE_DIRS} )
else()
	message( FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No Poco C++ Libraries found!" )
endif()

find_package( FMT REQUIRED )
if( FMT_FOUND )
	include_directories( ${FMT_INCLUDE_DIR} )
else()
	message( FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No FMT library found!" )
endif()

include_directories( ${CMAKE_SOURCE_DIR}/include )

if ( CIC_BUILD_TASK_LIB OR CIC_BUILD_CICHECK )
	add_subdirectory( task )
endif()

if ( CIC_BUILD_CICHECK )
	add_subdirectory( cicheck )
endif()

