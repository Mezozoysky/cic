find_package( Poco REQUIRED Foundation XML JSON Util )
if( Poco_FOUND )
	include_directories( ${Poco_INCLUDE_DIRS} )
else()
	message( FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No Poco C++ Libraries found!" )
endif()

include_directories( ${CMAKE_SOURCE_DIR}/include )

if ( CIC_BUILD_TGT_LIB OR CIC_BUILD_CICHECK )
	add_subdirectory( tgt )
endif()

if ( CIC_BUILD_CICHECK )
	add_subdirectory( cicheck )
endif()

