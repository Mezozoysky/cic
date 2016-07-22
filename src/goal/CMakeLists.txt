# goal lib

set( LIB_NAME "cic-goal" )

set( LIB_SRCTREE_ROOT ${CMAKE_SOURCE_DIR}/src/goal )
set( LIB_HEADERS_ROOT ${LIB_SRCTREE_ROOT}/include/CICheck/goal )
set( LIB_SOURCES_ROOT ${LIB_SRCTREE_ROOT}/src )

include_directories( ${LIB_SRCTREE_ROOT}/include )

set( LIB_HEADERS "" )
set( LIB_SOURCES "" )

# ====================
# ==== goal group ====
# ====================
set(GROUP_HEADERS
	${LIB_HEADERS_ROOT}/Goal.hpp
	${LIB_HEADERS_ROOT}/ARule.hpp
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
