# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mge_opengl_module_SOURCES
    render_system.cpp
    window.cpp
    render_context.cpp
    opengl_info.cpp
    index_buffer.cpp
    vertex_buffer.cpp
    shader.cpp
    error.cpp
    common.cpp
    program.cpp
    texture.cpp
    slang_compiler.cpp)

ADD_LIBRARY(mge_module_opengl SHARED
            ${mge_opengl_module_SOURCES})
TARGET_INCLUDE_DIRECTORIES(mge_module_opengl PRIVATE ${OPENGL_INCLUDE_DIRS})
IF(WIN32)
TARGET_LINK_LIBRARIES(mge_module_opengl
    mge_win32
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    gl3w
    slang::slang
    ${OPENGL_LIBRARIES})
ELSE()
TARGET_LINK_LIBRARIES(mge_module_opengl
    mge_glfw
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    gl3w
    slang::slang
    ${OPENGL_LIBRARIES})
ENDIF()
ADD_SUBDIRECTORY(test)
