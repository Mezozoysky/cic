# xgoal lib

set( LIB_NAME "xgoal" )
set( LIB_FILE_NAME "cic-xgoal" )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../goal/include )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../xmlu/include )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../industry/include )


set( LIB_HEADERS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include )
set( LIB_SOURCES_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src )


set( LIB_HEADERS "" )
set( LIB_SOURCES "" )

# ===========================
# ==== ${LIB_NAME} group ====
# ===========================
set(GROUP_HEADERS
	${LIB_HEADERS_ROOT}/${CMAKE_PROJECT_NAME}/${LIB_NAME}/ARule.hpp
	${LIB_HEADERS_ROOT}/${CMAKE_PROJECT_NAME}/${LIB_NAME}/Goal.hpp
	${LIB_HEADERS_ROOT}/${CMAKE_PROJECT_NAME}/${LIB_NAME}/Provider.hpp
)
set(GROUP_SOURCES
	${LIB_SOURCES_ROOT}/Goal.cpp
	${LIB_SOURCES_ROOT}/Provider.cpp
)
source_group( ${LIB_NAME} FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( LIB_HEADERS ${LIB_HEADERS} ${GROUP_HEADERS} )
set( LIB_SOURCES ${LIB_SOURCES} ${GROUP_SOURCES} )

add_library( ${LIB_FILE_NAME} STATIC
	${LIB_HEADERS}
	${LIB_SOURCES}
)
set_target_properties( ${LIB_FILE_NAME} PROPERTIES LINKER_LANGUAGE CXX )
target_include_directories( ${LIB_FILE_NAME}
	PUBLIC
	$<BUILD_INTERFACE:${LIB_HEADERS_ROOT}>
	$<INSTALL_INTERFACE:include>
	PRIVATE
	${LIB_SOURCES_ROOT}
)


if( CIC_BUILD_GOAL_LIB )
	install(
		TARGETS ${LIB_FILE_NAME}
		ARCHIVE DESTINATION lib
	)
	install(
		FILES ${LIB_HEADERS}
		DESTINATION include/${CMAKE_PROJECT_NAME}/${LIB_NAME}
	)
endif()
