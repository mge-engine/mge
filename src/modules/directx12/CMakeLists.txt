# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mge_dx12_module_SOURCES
    common.cpp
    window.cpp
    error.cpp
    input_layout_cache.cpp
    render_system.cpp
    render_context.cpp
    shader.cpp
    program.cpp
    texture.cpp
    vertex_buffer.cpp
    index_buffer.cpp
)

ADD_LIBRARY(mge_module_dx12 MODULE
    ${mge_dx12_module_SOURCES})

TARGET_LINK_LIBRARIES(mge_module_dx12 mgecore mgegraphics mge_win32 mgeapplication dx12)
ADD_SUBDIRECTORY(test)
