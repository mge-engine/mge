# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_SUBDIRECTORY(vk)
SET(mge_vulkan_module_SOURCES
    common.cpp
    window.cpp
    render_system.cpp)
ADD_LIBRARY(mge_module_vulkan MODULE
    ${mge_vulkan_module_SOURCES})
IF(WIN32)
    TARGET_LINK_LIBRARIES(mge_module_vulkan mge_win32 mgeapplication)
ENDIF()
TARGET_LINK_LIBRARIES(mge_module_vulkan mgecore mgegraphics vkutils vulkan)


    
