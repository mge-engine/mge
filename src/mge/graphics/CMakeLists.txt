# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgegraphics_sources
    render_system.cpp
    extent.cpp
    point.cpp
    window_options.cpp
    window.cpp
    monitor.cpp
    video_mode.cpp
    render_context.cpp
    shader_type.cpp
    shader_language.cpp
    context_object.cpp
    command_list.cpp
    color_names.cpp
    rgb_color.cpp
    rgba_color.cpp
    buffer_type.cpp
    hardware_buffer.cpp
    data_type.cpp
    index_buffer.cpp
    swap_chain.cpp
    vertex_format.cpp
    vertex_layout.cpp
    vertex_buffer.cpp
    shader.cpp
    program.cpp
    texture.cpp
    shader_format.cpp
    draw_command.cpp
    uniform_data_type.cpp
    attribute_semantic.cpp
    command_sequence.cpp
)

SET(mgegraphics_headers
    dllexport.hpp
    render_system.hpp
    extent.hpp
    point.hpp
    window_options.hpp
    window.hpp
    monitor.hpp
    video_mode.hpp
    render_context.hpp
    shader_type.hpp
    shader_language.hpp
    context_object.hpp
    command_list.hpp
    rgb_color.hpp
    rgba_color.hpp
    buffer_type.hpp
    hardware_buffer.hpp
    data_type.hpp
    index_buffer.hpp
    swap_chain.hpp
    vertex_format.hpp
    vertex_layout.hpp
    vertex_buffer.hpp
    shader.hpp
    program.hpp
    shader_format.hpp
    draw_command.hpp
    uniform_data_type.hpp
    attribute_semantic.hpp
)

MGE_LIBRARY(
    TARGET  mgegraphics
    TYPE    SHARED
    SOURCES
    ${mgegraphics_sources}
    ${mgegraphics_headers}
)

TARGET_LINK_LIBRARIES(mgegraphics mgecore mgeinput boost-all)
ADD_SUBDIRECTORY(test)
