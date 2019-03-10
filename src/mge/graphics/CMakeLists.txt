# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mgegraphics
    SHARED
    render_system.cpp
    extent.cpp
    rectangle.cpp
    gamma_ramp.cpp
    monitor.cpp
    window.cpp
    render_target.cpp
    window_options.cpp)

TARGET_ADD_DEFINE(mgegraphics BUILD_MGE_GRAPHICS)
TARGET_LINK_LIBRARIES(mgegraphics mgecore mgeinput mgeapplication)

ADD_SUBDIRECTORY(test)
