# tgt lib

set( TGT_LIB_HEADERS "" )
set( TGT_LIB_SOURCES "" )

# ===================
# ==== tgt group ====
# ===================
set(GROUP_HEADERS
	${CMAKE_SOURCE_DIR}/include/CICheck/tgt/Target.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/tgt/TargetProvider.hpp
)
set(GROUP_SOURCES
	${CMAKE_SOURCE_DIR}/src/tgt/Target.cpp
	${CMAKE_SOURCE_DIR}/src/tgt/TargetProvider.cpp
)
source_group( "tgt" FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( TGT_LIB_HEADERS ${TGT_LIB_HEADERS} ${GROUP_HEADERS} )
set( TGT_LIB_SOURCES ${TGT_LIB_SOURCES} ${GROUP_SOURCES} )

add_library( tgt STATIC
	${TGT_LIB_HEADERS}
	${TGT_LIB_SOURCES}
)
set_target_properties( tgt PROPERTIES LINKER_LANGUAGE CXX )

if( CIC_BUILD_TGT_LIB )
	install(
		TARGETS tgt
		ARCHIVE DESTINATION lib
	)
	install(
		FILES ${TGT_LIB_HEADERS}
		DESTINATION include/CICheck/tgt
	)
endif()
