include_directories( ${CMAKE_SOURCE_DIR}/deps/include )

if( CIC_BUILD_SCRIPTING_LIB OR CIC_BUILD_CHECK_APP )
    add_subdirectory( cic-scripting )
endif()

if( CIC_BUILD_CHECK_LIB OR CIC_BUILD_CHECK_APP)
	add_subdirectory( cic-check )
endif()

if( CIC_BUILD_CHECK_APP )
	add_subdirectory( check )
endif()
