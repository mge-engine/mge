# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mgegraphics
    SHARED
    color_names.cpp
    rgb_color.cpp
    rgba_color.cpp
    render_system.cpp
    extent.cpp
    rectangle.cpp
    gamma_ramp.cpp
    monitor.cpp
    window.cpp
    render_context.cpp
    context_object.cpp
    hardware_buffer.cpp
    command_list.cpp
    memory_command_list.cpp
    window_options.cpp)

TARGET_ADD_DEFINE(mgegraphics BUILD_MGE_GRAPHICS)
TARGET_LINK_LIBRARIES(mgegraphics mgecore mgeinput mgeapplication)

ADD_SUBDIRECTORY(test)
