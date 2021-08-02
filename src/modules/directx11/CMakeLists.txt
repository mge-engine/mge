# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
SET(mge_dx11_module_SOURCES
    common.cpp
    error.cpp
    window.cpp
    render_context.cpp
    render_system.cpp
)

ADD_LIBRARY(mge_module_dx11 MODULE
    ${mge_dx11_module_SOURCES})

TARGET_LINK_LIBRARIES(mge_module_dx11 mgecore mgegraphics mge_win32 mgeapplication dx11)
#ADD_SUBDIRECTORY(test)
