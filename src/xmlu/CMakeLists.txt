# xmlu lib

set( XMLU_LIB_HEADERS "" )
set( XMLU_LIB_SOURCES "" )

# ====================
# ==== xmlu group ====
# ====================
set(GROUP_HEADERS
	${CMAKE_SOURCE_DIR}/include/CICheck/xmlu/XMLUtils.hpp
)
set(GROUP_SOURCES
	${CMAKE_SOURCE_DIR}/src/xmlu/XMLUtils.cpp
)
source_group( "xmlu" FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( XMLU_LIB_HEADERS ${XMLU_LIB_HEADERS} ${GROUP_HEADERS} )
set( XMLU_LIB_SOURCES ${XMLU_LIB_SOURCES} ${GROUP_SOURCES} )

add_library( cic-xmlu STATIC
	${XMLU_LIB_HEADERS}
	${XMLU_LIB_SOURCES}
)
set_target_properties( cic-xmlu PROPERTIES LINKER_LANGUAGE CXX )

if( CIC_BUILD_XMLU_LIB )
	install(
		TARGETS cic-xmlu
		ARCHIVE DESTINATION lib
	)
	install(
		FILES ${XMLU_LIB_HEADERS}
		DESTINATION include/CICheck/xmlu
	)
endif()
