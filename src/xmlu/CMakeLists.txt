# industry lib

set( LIB_NAME "xmlu" )
set( LIB_FILE_NAME "cic-xmlu")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../industry/include )

set( LIB_HEADERS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include )
set( LIB_SOURCES_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src )


set( LIB_HEADERS "" )
set( LIB_SOURCES "" )

# ===========================
# ==== ${LIB_NAME} group ====
# ===========================
set(GROUP_HEADERS
	${LIB_HEADERS_ROOT}/${CMAKE_PROJECT_NAME}/${LIB_NAME}/XMLUtils.hpp
	${LIB_HEADERS_ROOT}/${CMAKE_PROJECT_NAME}/${LIB_NAME}/LoadableFromXML.hpp
)
set(GROUP_SOURCES
	${LIB_SOURCES_ROOT}/XMLUtils.cpp
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


if( CIC_BUILD_INDUSTRY_LIB )
	install(
		TARGETS ${LIB_FILE_NAME}
		ARCHIVE DESTINATION lib
	)
	install(
		FILES ${LIB_HEADERS}
		DESTINATION include/${CMAKE_PROJECT_NAME}/${LIB_NAME}
	)
endif()
