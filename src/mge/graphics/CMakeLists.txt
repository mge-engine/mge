# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
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
