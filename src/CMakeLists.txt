include_directories( ${CMAKE_SOURCE_DIR}/deps/include )

if( CIC_BUILD_INDUSTRY_LIB OR CIC_BUILD_PLAN_LIB OR CIC_BUILD_CHECK_APP )
    add_subdirectory( cic-industry )
endif()

if( CIC_BUILD_SCRIPTING_LIB OR CIC_BUILD_CHECK_APP )
    add_subdirectory( cic-scripting )
endif()

if( CIC_BUILD_PLAN_LIB OR CIC_BUILD_CHECK_APP )
    add_subdirectory( cic-plan )
endif()

if( CIC_BUILD_CHECK_APP )
    add_subdirectory( check )
endif()
