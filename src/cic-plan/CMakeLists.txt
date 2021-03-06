# cic-plan lib

set( MODULE_NAME "cic-plan" )
set( MODULE_NSPATH "cic/plan" )
set( MODULE_TGT "cic-plan")
set( MODULE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" )

set( MODULE_HEADERS "" )
set( MODULE_SOURCES "" )

# ===============================================
# ==== ${MODULE_NAME}/${MODULE_NSPATH} group ====
# ===============================================
set(GROUP_HEADERS
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/DAGNode.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/Act.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/ActFailure.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/ActSuccess.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/ActShell.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/Plan.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/LinearPlan.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/DepsTreePlan.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/Phase.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/DepsTreePhase.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/Report.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/PerformConfig.hpp"
    "${MODULE_ROOT}/include/${MODULE_NSPATH}/Target.hpp"
    )
set(GROUP_SOURCES
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/Act.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/ActFailure.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/ActSuccess.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/ActShell.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/Plan.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/LinearPlan.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/DepsTreePlan.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/Phase.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/DepsTreePhase.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/Report.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/PerformConfig.cpp"
    "${MODULE_ROOT}/src/${MODULE_NSPATH}/Target.cpp"
    )
source_group( ${MODULE_NAME} FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( MODULE_HEADERS ${MODULE_HEADERS} ${GROUP_HEADERS} )
set( MODULE_SOURCES ${MODULE_SOURCES} ${GROUP_SOURCES} )

include_directories(
    ${MODULE_ROOT}/include
    ${MODULE_ROOT}/src
    $<TARGET_PROPERTY:Poco::XML,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Poco::JSON,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Poco::Foundation,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:cic-industry,INTERFACE_INCLUDE_DIRECTORIES>
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
    INTERFACE Poco::XML Poco::JSON Poco::Foundation fmt::fmt cic-industry
    )
target_include_directories( ${MODULE_TGT}
    PUBLIC
    $<BUILD_INTERFACE:${MODULE_ROOT}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${MODULE_ROOT}/src
    INTERFACE
    $<TARGET_PROPERTY:Poco::XML,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Poco::JSON,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Poco::Foundation,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:cic-industry,INTERFACE_INCLUDE_DIRECTORIES>
    )

#
# Install
#
if ( CIC_BUILD_PLAN_LIB )
    install(
        TARGETS ${MODULE_TGT}
        ARCHIVE DESTINATION lib
        )
    install(
        DIRECTORY ${MODULE_ROOT}/share/cic/${MODULE_NAME}
        DESTINATION share/cic
        )
endif()
