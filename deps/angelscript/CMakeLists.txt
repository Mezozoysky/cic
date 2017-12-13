set(MODULE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(ANGELSCRIPT_LIBRARY_NAME angelscript)

set(ANGELSCRIPT_HEADERS_ROOT "${MODULE_ROOT}/include")
set(ANGELSCRIPT_SOURCES_ROOT "${MODULE_ROOT}/source")
set(ANGELSCRIPT_ADDONS_ROOT "${MODULE_ROOT}/add_on")

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

file(READ ${ANGELSCRIPT_HEADERS_ROOT}/angelscript.h ANGELSCRIPT_H)
string(REGEX MATCH "#define ANGELSCRIPT_VERSION_STRING \"([0-9]*).([0-9]*).([0-9]*)" ANGELSCRIPT_VERSION_REGEX ${ANGELSCRIPT_H})
set(ANGELSCRIPT_VERSION_MAJOR ${CMAKE_MATCH_1})
set(ANGELSCRIPT_VERSION_MINOR ${CMAKE_MATCH_2})
set(ANGELSCRIPT_VERSION_PATCH ${CMAKE_MATCH_3})
set(PROJECT_VERSION ${ANGELSCRIPT_VERSION_MAJOR}.${ANGELSCRIPT_VERSION_MINOR}.${ANGELSCRIPT_VERSION_PATCH})

message(STATUS "Configuring angelscript ${PROJECT_VERSION}")


set(ANGELSCRIPT_HEADERS
    ${ANGELSCRIPT_HEADERS_ROOT}/angelscript.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_array.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_builder.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_bytecode.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_compiler.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_config.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_configgroup.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_context.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_criticalsection.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_datatype.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_debug.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_generic.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_map.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_memory.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_module.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_objecttype.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_outputbuffer.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_parser.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_property.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_restore.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptcode.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptengine.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptfunction.h

    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptnode.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptobject.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_string.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_string_util.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_texts.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_thread.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_tokendef.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_tokenizer.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_typeinfo.h
    ${ANGELSCRIPT_SOURCES_ROOT}/as_variablescope.h
    )

set(ANGELSCRIPT_SOURCE
    ${ANGELSCRIPT_SOURCES_ROOT}/as_atomic.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_builder.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_bytecode.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc_mips.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc_x86.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc_x64_gcc.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc_x64_msvc.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc_x64_mingw.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_compiler.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_configgroup.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_context.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_datatype.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_gc.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_generic.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_globalproperty.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_memory.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_module.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_objecttype.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_outputbuffer.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_parser.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_restore.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptcode.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptengine.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptfunction.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptnode.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_scriptobject.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_string.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_string_util.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_thread.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_tokenizer.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_typeinfo.cpp
    ${ANGELSCRIPT_SOURCES_ROOT}/as_variablescope.cpp
    )

if(MSVC AND CMAKE_CL_64)
    enable_language(ASM_MASM)
    if(CMAKE_ASM_MASM_COMPILER_WORKS)
        set(ANGELSCRIPT_SOURCE ${ANGELSCRIPT_SOURCE} ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc_x64_msvc_asm.asm)
    else()
        message(FATAL ERROR "MSVC x86_64 target requires a working assembler")
    endif()
endif()

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm")
    enable_language(ASM)
    if(CMAKE_ASM_COMPILER_WORKS)
        set(ANGELSCRIPT_SOURCE ${ANGELSCRIPT_SOURCE} ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc_arm.cpp ../../source/as_callfunc_arm_gcc.S)
        set_property(SOURCE ${ANGELSCRIPT_SOURCES_ROOT}/as_callfunc_arm_gcc.S APPEND PROPERTY COMPILE_FLAGS " -Wa,-mimplicit-it=always")
    else()
        message(FATAL ERROR "ARM target requires a working assembler")
    endif()
endif()

include_directories(
    ${ANGELSCRIPT_HEADERS_ROOT}
    )

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_definitions(-DANGELSCRIPT_EXPORT -D_LIB)

# Fix x64 issues on Linux
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" AND NOT APPLE)
    add_definitions(-fPIC)
endif()

add_library(${ANGELSCRIPT_LIBRARY_NAME} ${ANGELSCRIPT_SOURCE} ${ANGELSCRIPT_HEADERS})

set_target_properties(${ANGELSCRIPT_LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

if(MSVC)
    set_target_properties(${ANGELSCRIPT_LIBRARY_NAME} PROPERTIES COMPILE_FLAGS "/MP")
endif()

target_include_directories(${ANGELSCRIPT_LIBRARY_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${ANGELSCRIPT_HEADERS_ROOT}>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${ANGELSCRIPT_SOURCES_ROOT}
    )
