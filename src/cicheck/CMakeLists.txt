# cicheck

#
# OS-specific libs
#
set( CICHECK_OS_SPECIFIC_LIBS "" )

if( CIC_OS_LINUX )
    find_library( PTHREAD_LIBRARY NAMES pthread )
    set( CICHECK_OS_SPECIFIC_LIBS ${CICHECK_OS_SPECIFIC_LIBS}
        ${PTHREAD_LIBRARY}
    )
endif()

if( CIC_OS_OSX )
    set( CICHECK_OS_SPECIFIC_LIBS ${CICHECK_TOOL_OS_SPECIFIC_LIBS}
    )
endif()

if( CIC_OS_WINDOWS )
	set( CICHECK_OS_SPECIFIC_LIBS ${CICHECK_OS_SPECIFIC_LIBS}
    )
endif()


#
# Sources
#
set( CICHECK_HEADERS "" )
set( CICHECK_SOURCES "" )

# =======================
# ==== cicheck group ====
# =======================
set(GROUP_HEADERS
	${CMAKE_SOURCE_DIR}/src/cicheck/Application.hpp
    ${CMAKE_SOURCE_DIR}/src/cicheck/Rules.hpp
)
set(GROUP_SOURCES
	${CMAKE_SOURCE_DIR}/src/cicheck/main.cpp
	${CMAKE_SOURCE_DIR}/src/cicheck/Application.cpp
    ${CMAKE_SOURCE_DIR}/src/cicheck/Rules.cpp
)
source_group( "cicheck" FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( CICHECK_HEADERS ${CICHECK_HEADERS} ${GROUP_HEADERS} )
set( CICHECK_SOURCES ${CICHECK_SOURCES} ${GROUP_SOURCES} )

add_executable( cicheck WIN32 #MACOSX_BUNDLE
    ${CICHECK_HEADERS}
    ${CICHECK_SOURCES}
)

#
# Build
#
target_link_libraries( cicheck
	tgt
)
target_link_libraries( cicheck
	${FMT_LIBRARY}
)
target_link_libraries( cicheck
	${Poco_Util_LIBRARY}
	${Poco_XML_LIBRARY}
	${Poco_JSON_LIBRARY}  
	${Poco_Foundation_LIBRARY}
)
target_link_libraries( cicheck ${CIC_OS_SPECIFIC_LIBS} )

#add_custom_command(
#    TARGET cicheck POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy
#    ${CMAKE_SOURCE_DIR}/cicheck.properties $<TARGET_FILE_DIR:cicheck>
#)

#
# Install
#
install(
    TARGETS cicheck
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin
)
