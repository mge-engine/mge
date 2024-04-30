# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mge_opengl_module_SOURCES
    render_system.cpp
    window.cpp
    render_context.cpp
    opengl_info.cpp
    swap_chain.cpp
    index_buffer.cpp
    vertex_buffer.cpp
    command_list.cpp
    shader.cpp
    error.cpp
    common.cpp
    program.cpp
    texture.cpp)

ADD_LIBRARY(mge_module_opengl MODULE
            ${mge_opengl_module_SOURCES})
TARGET_INCLUDE_DIRECTORIES(mge_module_opengl PRIVATE ${OPENGL_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(mge_module_opengl
    mge_win32
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    gl3w
    ${OPENGL_LIBRARIES})
ADD_SUBDIRECTORY(test)
