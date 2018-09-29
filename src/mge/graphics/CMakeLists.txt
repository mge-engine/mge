# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mgegraphics
    SHARED
    render_system.cpp)

TARGET_ADD_DEFINE(mgegraphics BUILD_MGE_GRAPHICS)
TARGET_LINK_LIBRARIES(mgegraphics mgecore)

#ADD_SUBDIRECTORY(test)
