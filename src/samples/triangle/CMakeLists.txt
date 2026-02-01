# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(triangle
               triangle.cpp)

TARGET_LINK_LIBRARIES(triangle
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics
    mgeasset)

MGE_CAPTURE_TEST(TARGET triangle
                 RENDER_SYSTEMS opengl directx11 directx12)
