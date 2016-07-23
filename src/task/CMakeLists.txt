# task lib

include_directories( ${CMAKE_SOURCE_DIR}/include )

set( TASK_LIB_HEADERS "" )
set( TASK_LIB_SOURCES "" )

# ====================
# ==== task group ====
# ====================
set(GROUP_HEADERS
	${CMAKE_SOURCE_DIR}/include/CICheck/task/TaskUnit.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/ATask.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/ATargetSet.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/ACheckMap.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/ARuleSet.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/ARule.hpp
)
set(GROUP_SOURCES
	${CMAKE_SOURCE_DIR}/src/task/TaskUnit.cpp
	${CMAKE_SOURCE_DIR}/src/task/ATask.cpp
	${CMAKE_SOURCE_DIR}/src/task/ATargetSet.cpp
	${CMAKE_SOURCE_DIR}/src/task/ACheckMap.cpp
	${CMAKE_SOURCE_DIR}/src/task/ARuleSet.cpp
	${CMAKE_SOURCE_DIR}/src/task/ARule.cpp
)
source_group( "task" FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( TASK_LIB_HEADERS ${TASK_LIB_HEADERS} ${GROUP_HEADERS} )
set( TASK_LIB_SOURCES ${TASK_LIB_SOURCES} ${GROUP_SOURCES} )

add_library( cic-task STATIC
	${TASK_LIB_HEADERS}
	${TASK_LIB_SOURCES}
)
set_target_properties( cic-task PROPERTIES LINKER_LANGUAGE CXX )

if( CIC_BUILD_TASK_LIB )
	install(
		TARGETS cic-task
		ARCHIVE DESTINATION lib
	)
	install(
		FILES ${TASK_LIB_HEADERS}
		DESTINATION include/CICheck/task
	)
endif()
