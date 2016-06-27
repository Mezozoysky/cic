# task lib

set( TASK_LIB_HEADERS "" )
set( TASK_LIB_SOURCES "" )

# ====================
# ==== task group ====
# ====================
set(GROUP_HEADERS
	${CMAKE_SOURCE_DIR}/include/CICheck/task/TaskDecl.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/Task.hpp
	${CMAKE_SOURCE_DIR}/include/CICheck/task/TaskProvider.hpp
)
set(GROUP_SOURCES
	${CMAKE_SOURCE_DIR}/src/task/TaskDecl.cpp
	${CMAKE_SOURCE_DIR}/src/task/Task.cpp
	${CMAKE_SOURCE_DIR}/src/task/TaskProvider.cpp
)
source_group( "task" FILES ${GROUP_HEADERS} ${GROUP_SOURCES} )
set( TASK_LIB_HEADERS ${TASK_LIB_HEADERS} ${GROUP_HEADERS} )
set( TASK_LIB_SOURCES ${TASK_LIB_SOURCES} ${GROUP_SOURCES} )

add_library( task STATIC
	${TASK_LIB_HEADERS}
	${TASK_LIB_SOURCES}
)
set_target_properties( task PROPERTIES LINKER_LANGUAGE CXX )

if( CIC_BUILD_TASK_LIB )
	install(
		TARGETS task
		ARCHIVE DESTINATION lib
	)
	install(
		FILES ${TASK_LIB_HEADERS}
		DESTINATION include/CICheck/task
	)
endif()
