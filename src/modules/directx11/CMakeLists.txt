# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mge_dx11_module_SOURCES
    common.cpp
    error.cpp
    window.cpp
    render_context.cpp
    render_system.cpp
    swap_chain.cpp
    index_buffer.cpp
    vertex_buffer.cpp
    command_list.cpp
    shader.cpp
    program.cpp
    input_layout_cache.cpp
    texture.cpp
)

ADD_LIBRARY(mge_module_dx11 MODULE
    ${mge_dx11_module_SOURCES})

TARGET_LINK_LIBRARIES(mge_module_dx11 
    mgecore 
    mgegraphics 
    mgeapplication
    
    mge_win32 
    
    dx11)
ADD_SUBDIRECTORY(test)
