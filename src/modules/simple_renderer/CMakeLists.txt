# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.

SET(mge_module_simple_renderer_SOURCES
    simple_renderer.cpp
)

ADD_LIBRARY(mge_module_simple_renderer SHARED ${mge_module_simple_renderer_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_simple_renderer mgecore mgegraphics mgescene)