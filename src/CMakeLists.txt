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

if( CIC_BUILD_TU_LIB OR CIC_BUILD_CICHECK )
	add_subdirectory( tu )
	include_directories( ${CMAKE_SOURCE_DIR}/include )
endif()

if( CIC_BUILD_INDUSTRY_LIB OR CIC_BUILD_CHECK )
	add_subdirectory( industry )
endif()

if( CIC_BUILD_XMLU_LIB OR CIC_BUILD_CICHECK )
	add_subdirectory( xmlu )
	include_directories( ${CMAKE_SOURCE_DIR}/include )
endif()

if( CIC_BUILD_TASK_LIB OR CIC_BUILD_CICHECK )
	add_subdirectory( task )
	include_directories( ${CMAKE_SOURCE_DIR}/include )
endif()

if( CIC_BUILD_GOAL_LIB OR CIC_BUILD_CICHECK OR CIC_BUILD_CHECK )
	add_subdirectory( goal )
endif()

if( CIC_BUILD_CICHECK )
	add_subdirectory( cicheck )
endif()

if( CIC_BUILD_CHECK )
	add_subdirectory( check )
endif()
