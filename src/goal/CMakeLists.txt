# goal lib

set( LIB_NAME "cic-goal" )

set( LIB_HEADERS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include )
set( LIB_SOURCES_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src )


set( LIB_HEADERS "" )
set( LIB_SOURCES "" )

# ====================
# ==== goal group ====
# ====================
set(GROUP_HEADERS
	${LIB_HEADERS_ROOT}/CICheck/goal/Goal.hpp
	${LIB_HEADERS_ROOT}/CICheck/goal/ARule.hpp
)
set(GROUP_SOURCES
	${LIB_SOURCES_ROOT}/Goal.cpp
	${LIB_SOURCES_ROOT}/ARule.cpp
)
source_group( "goal" FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( LIB_HEADERS ${LIB_HEADERS} ${GROUP_HEADERS} )
set( LIB_SOURCES ${LIB_SOURCES} ${GROUP_SOURCES} )

add_library( ${LIB_NAME} STATIC
	${LIB_HEADERS}
	${LIB_SOURCES}
)
set_target_properties( ${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX )
target_include_directories( ${LIB_NAME}
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)


if( CIC_BUILD_GOAL_LIB )
	install(
		TARGETS ${LIB_NAME}
		ARCHIVE DESTINATION lib
	)
	install(
		FILES ${LIB_HEADERS}
		DESTINATION include/CICheck/goal
	)
endif()
