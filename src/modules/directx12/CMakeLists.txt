# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mge_dx12_module_SOURCES
    common.cpp
    window.cpp
    error.cpp
    render_system.cpp
    render_context.cpp
    swap_chain.cpp
    command_list.cpp
    frame_command_list.cpp
    shader.cpp
    program.cpp
    vertex_buffer.cpp
    index_buffer.cpp
)

ADD_LIBRARY(mge_module_dx12 MODULE
    ${mge_dx12_module_SOURCES})

TARGET_LINK_LIBRARIES(mge_module_dx12 mgecore mgegraphics mge_win32 mgeapplication dx12)
ADD_SUBDIRECTORY(test)
