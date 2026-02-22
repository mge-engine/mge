# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mge_module_renderdoc_SOURCES
    renderdoc.cpp
)

ADD_LIBRARY(mge_module_renderdoc SHARED ${mge_module_renderdoc_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_renderdoc mgecore mgegraphics)