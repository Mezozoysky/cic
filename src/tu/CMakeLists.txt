# tu lib

set( TU_LIB_HEADERS "" )
set( TU_LIB_SOURCES "" )

# ==================
# ==== tu group ====
# ==================
set(GROUP_HEADERS
	${CMAKE_SOURCE_DIR}/include/CICheck/tu/Factory.hpp
)
set(GROUP_SOURCES
	${CMAKE_SOURCE_DIR}/src/tu/fake.cpp
)
source_group( "tu" FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( TU_LIB_HEADERS ${TU_LIB_HEADERS} ${GROUP_HEADERS} )
set( TU_LIB_SOURCES ${TU_LIB_SOURCES} ${GROUP_SOURCES} )

add_library( cic-tu-fake STATIC
	${TU_LIB_HEADERS}
	${TU_LIB_SOURCES}
)
set_target_properties( cic-tu-fake PROPERTIES LINKER_LANGUAGE CXX )

if( CIC_BUILD_TU_LIB )
	install(
		FILES ${TU_LIB_HEADERS}
		DESTINATION include/CICheck/tu
	)
endif()
