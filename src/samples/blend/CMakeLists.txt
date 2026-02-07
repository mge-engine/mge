# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(blend
               blend.cpp)

TARGET_LINK_LIBRARIES(blend
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics
    mgeasset)

MGE_CAPTURE_TEST(TARGET blend
                 RENDER_SYSTEMS opengl directx11 directx12 vulkan)
