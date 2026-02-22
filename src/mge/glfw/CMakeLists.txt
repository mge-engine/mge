# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mge_glfw STATIC
    monitor.cpp
    window.cpp)

TARGET_LINK_LIBRARIES(mge_glfw mgecore mgegraphics mgeapplication mgeinput glfw)
ADD_SUBDIRECTORY(test)
