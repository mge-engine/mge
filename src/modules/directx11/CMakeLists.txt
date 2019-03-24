# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
# Copyright (c) 2017 by Alexander Schroeder
SET(mge_dx11_module_SOURCES
    common.cpp
    render_system.cpp
    render_context.cpp
    window.cpp
)
ADD_LIBRARY(mge_module_dx11 MODULE
    ${mge_dx11_module_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_dx11 mgecore mgegraphics mge_win32 mgeapplication dxgi d3dcompiler dxguid)
#ADD_SUBDIRECTORY(test)

