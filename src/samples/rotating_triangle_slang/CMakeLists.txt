# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(rotating_triangle_slang
               rotating_triangle_slang.cpp)

TARGET_LINK_LIBRARIES(rotating_triangle_slang
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics
    mgeasset
    mgemath)

MGE_CAPTURE_TEST(TARGET rotating_triangle_slang RENDER_SYSTEMS opengl directx11 directx12 vulkan)
