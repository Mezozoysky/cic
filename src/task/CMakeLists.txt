# task lib

set( TASK_LIB_HEADERS "" )
set( TASK_LIB_SOURCES "" )

# ====================
# ==== task group ====
# ====================
set(GROUP_HEADERS
	${CMAKE_SOURCE_DIR}/include/CICheck/task/TaskDecl.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/AbstractTask.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/Task.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/Target.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/AbstractTargetSet.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/LinearTargetSet.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/CheckMap.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/RuleSet.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/AbstractRule.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/BashScriptRule.hpp
)
set(GROUP_SOURCES
	${CMAKE_SOURCE_DIR}/src/task/TaskDecl.cpp
	${CMAKE_SOURCE_DIR}/src/task/AbstractTask.cpp
	${CMAKE_SOURCE_DIR}/src/task/Task.cpp
	${CMAKE_SOURCE_DIR}/src/task/Target.cpp
	${CMAKE_SOURCE_DIR}/src/task/AbstractTargetSet.cpp
	${CMAKE_SOURCE_DIR}/src/task/LinearTargetSet.cpp
	${CMAKE_SOURCE_DIR}/src/task/CheckMap.cpp
	${CMAKE_SOURCE_DIR}/src/task/RuleSet.cpp
	${CMAKE_SOURCE_DIR}/src/task/AbstractRule.cpp
	${CMAKE_SOURCE_DIR}/src/task/BashScriptRule.cpp
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
