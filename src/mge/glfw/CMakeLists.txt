# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mge_glfw STATIC
    glfw_instance.cpp
    monitor.cpp
    window.cpp)

TARGET_LINK_LIBRARIES(mge_glfw mgecore glfw)
ADD_SUBDIRECTORY(test)
