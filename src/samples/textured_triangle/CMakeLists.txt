# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(textured_triangle
               textured_triangle.cpp)

TARGET_LINK_LIBRARIES(textured_triangle
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics
    mgeasset)

MGE_CAPTURE_TEST(TARGET textured_triangle
                 RENDER_SYSTEMS opengl directx11 directx12 vulkan)
