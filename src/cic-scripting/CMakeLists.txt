# cic-scripting lib

set( MODULE_NAME "cic-scripting" )
set( MODULE_NSPATH "cic/scripting" )
set( MODULE_TGT "cic-scripting")
set( MODULE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" )

set( MODULE_HEADERS "" )
set( MODULE_SOURCES "" )

# ===============================================
# ==== ${MODULE_NAME}/${MODULE_NSPATH} group ====
# ===============================================
set(GROUP_HEADERS
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/Scripting.hpp"
    )
set(GROUP_SOURCES
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/Scripting.cpp"
    )
source_group( ${MODULE_NAME} FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( MODULE_HEADERS ${MODULE_HEADERS} ${GROUP_HEADERS} )
set( MODULE_SOURCES ${MODULE_SOURCES} ${GROUP_SOURCES} )

include_directories(
    ${MODULE_ROOT}/include
    ${MODULE_ROOT}/src
    $<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:angelscript,INTERFACE_INCLUDE_DIRECTORIES>
    )

#
# Build
#
add_library( ${MODULE_TGT} STATIC
    ${MODULE_HEADERS}
    ${MODULE_SOURCES}
    )
set_target_properties( ${MODULE_TGT} PROPERTIES LINKER_LANGUAGE CXX )
target_link_libraries( ${MODULE_TGT}
    INTERFACE fmt::fmt
    angelscript
    )
target_include_directories( ${MODULE_TGT}
    PUBLIC
    $<BUILD_INTERFACE:${MODULE_ROOT}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${MODULE_ROOT}/src
    INTERFACE
    #$<TARGET_PROPERTY:angelscript,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>
    )

#
# Install
#
if ( CIC_BUILD_SCRIPTING_LIB )
    install(
        TARGETS ${MODULE_TGT}
        ARCHIVE DESTINATION lib
        )
    install(
        DIRECTORY ${MODULE_ROOT}/share/cic/${MODULE_NAME}
        DESTINATION share/cic
        )
endif()
