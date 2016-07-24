# check app

set( APP_NAME "check" )
set( APP_FILE_NAME "cic-check" )

set( APP_HEADERS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src )
set( APP_SOURCES_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src )

#
# OS-specific libs
#
set( APP_OS_SPECIFIC_LIBS "" )

if( CIC_OS_LINUX )
	find_library( PTHREAD_LIBRARY NAMES pthread )
	set( APP_OS_SPECIFIC_LIBS ${APP_OS_SPECIFIC_LIBS}
		${PTHREAD_LIBRARY}
	)
endif()

if( CIC_OS_OSX )
	set( APP_OS_SPECIFIC_LIBS ${APP_OS_SPECIFIC_LIBS}
	)
endif()

if( CIC_OS_WINDOWS )
	set( APP_OS_SPECIFIC_LIBS ${APP_OS_SPECIFIC_LIBS}
	)
endif()

#
# Sources
#
set( APP_HEADERS "" )
set( APP_SOURCES "" )

# ===========================
# ==== ${APP_NAME} group ====
# ===========================
set(GROUP_HEADERS
	${APP_HEADERS_ROOT}/CheckApp.hpp
)
set(GROUP_SOURCES
	${APP_SOURCES_ROOT}/main.cpp
	${APP_SOURCES_ROOT}/CheckApp.cpp
)
source_group( ${APP_NAME} FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( APP_HEADERS ${APP_HEADERS} ${GROUP_HEADERS} )
set( APP_SOURCES ${APP_SOURCES} ${GROUP_SOURCES} )

#
# Build
#
add_executable( ${APP_FILE_NAME} WIN32 #MACOSX_BUNDLE
	${APP_HEADERS}
	${APP_SOURCES}
)

target_link_libraries( ${APP_FILE_NAME}
	cic-xgoal
	cic-goal
	cic-xmlu
	cic-industry-fake
)

target_link_libraries( ${APP_FILE_NAME}
	${FMT_LIBRARY}
)

target_link_libraries( ${APP_FILE_NAME}
	${Poco_Util_LIBRARY}
	${Poco_XML_LIBRARY}
	${Poco_JSON_LIBRARY}
	${Poco_Foundation_LIBRARY}
)

target_link_libraries( ${APP_FILE_NAME} ${APP_OS_SPECIFIC_LIBS} )

#
# Install
#
install(
	TARGETS ${APP_FILE_NAME}
	BUNDLE DESTINATION bin
	RUNTIME DESTINATION bin
)
