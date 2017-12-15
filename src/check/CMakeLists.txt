# check app

set( MODULE_NAME "check" )
set( MODULE_NSPATH "cic/check" )
set( MODULE_TGT "check" )
set( MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set( MODULE_HEADERS "" )
set( MODULE_SOURCES "" )

# ===============================================
# ==== ${MODULE_ROOT}/${MODULE_NSPATH} group ====
# ===============================================
set(GROUP_HEADERS
	${MODULE_ROOT}/src/CheckApp.hpp
)
set(GROUP_SOURCES
	${MODULE_ROOT}/src/main.cpp
	${MODULE_ROOT}/src/CheckApp.cpp
)
source_group( "${MODULE_NAME}\\${MODULE_NSPATH}"
    FILES ${GROUP_HEADERS} ${GROUP_SOURCES}
    )
set( MODULE_HEADERS ${MODULE_HEADERS} ${GROUP_HEADERS} )
set( MODULE_SOURCES ${MODULE_SOURCES} ${GROUP_SOURCES} )

#
# Build
#

include_directories(
    ${MODULE_ROOT}/src
    $<TARGET_PROPERTY:cic-plan,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:cic-scripting,INTERFACE_INCLUDE_DIRECTORIES>
    Threads::Threads
    )
set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
add_executable( ${MODULE_TGT} WIN32 #MACOSX_BUNDLE
	${MODULE_HEADERS}
	${MODULE_SOURCES}
)

target_link_libraries( ${MODULE_TGT}
    cic-plan
    cic-scripting
    Poco::Util
    Poco::XML
    Poco::JSON
    Poco::Foundation
    fmt::fmt
)

#
# Post-build
#

add_custom_command(
    TARGET ${MODULE_TGT}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${MODULE_ROOT}/share"
            "${CMAKE_BINARY_DIR}/share"
)
add_custom_command(
        TARGET ${MODULE_TGT}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${MODULE_ROOT}/etc"
                "${CMAKE_BINARY_DIR}/etc"
)

#
# Install
#

install(
	TARGETS ${MODULE_TGT}
	BUNDLE DESTINATION bin
	RUNTIME DESTINATION bin
)
install(
	DIRECTORY ${MODULE_ROOT}/share/cic/${MODULE_NAME}
	DESTINATION share/cic
)
