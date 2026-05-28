# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
SET(mge_module_forwardplus_SOURCES
    forwardplus.cpp
)

ADD_LIBRARY(mge_module_forwardplus SHARED ${mge_module_forwardplus_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_forwardplus mgecore mgegraphics mgescene)