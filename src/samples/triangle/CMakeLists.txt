# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(triangle
               triangle.cpp)

TARGET_LINK_LIBRARIES(triangle
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics)
