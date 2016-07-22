# goal lib

set( GOAL_LIB_PREFIX ${CMAKE_SOURCE_DIR}/src/goal )
set( GOAL_LIB_HEADERS_ROOT ${GOAL_LIB_PREFIX}/include/CICheck/goal )
set( GOAL_LIB_SOURCES_ROOT ${GOAL_LIB_PREFIX}/src )

include_directories( ${GOAL_LIB_PREFIX}/include )

set( GOAL_LIB_HEADERS "" )
set( GOAL_LIB_SOURCES "" )

# ====================
# ==== goal group ====
# ====================
set(GROUP_HEADERS
	${GOAL_LIB_HEADERS_ROOT}/Goal.hpp
	${GOAL_LIB_HEADERS_ROOT}/ARule.hpp
)
set(GROUP_SOURCES
	${GOAL_LIB_SOURCES_ROOT}/Goal.cpp
	${GOAL_LIB_SOURCES_ROOT}/ARule.cpp
)
source_group( "goal" FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( GOAL_LIB_HEADERS ${GOAL_LIB_HEADERS} ${GROUP_HEADERS} )
set( GOAL_LIB_SOURCES ${GOAL_LIB_SOURCES} ${GROUP_SOURCES} )

add_library( cic-goal STATIC
	${GOAL_LIB_HEADERS}
	${GOAL_LIB_SOURCES}
)
set_target_properties( cic-goal PROPERTIES LINKER_LANGUAGE CXX )

if( CIC_BUILD_GOAL_LIB )
	install(
		TARGETS cic-goal
		ARCHIVE DESTINATION lib
	)
	install(
		FILES ${GOAL_LIB_HEADERS}
		DESTINATION include/CICheck/goal
	)
endif()
