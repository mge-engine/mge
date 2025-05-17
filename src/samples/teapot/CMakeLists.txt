# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(teapot
               teapot.cpp)

TARGET_LINK_LIBRARIES(teapot
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics
    mgeasset)
