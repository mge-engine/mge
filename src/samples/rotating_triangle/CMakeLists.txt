# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(rotating_triangle
               rotating_triangle.cpp)

TARGET_LINK_LIBRARIES(rotating_triangle
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics
    mgeasset
    mgemath)

MGE_CAPTURE_TEST(TARGET rotating_triangle
                 RENDER_SYSTEMS opengl directx11 directx12 vulkan)
