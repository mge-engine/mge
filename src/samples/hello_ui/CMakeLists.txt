# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(hello_ui
               hello_ui.cpp)

TARGET_LINK_LIBRARIES(hello_ui
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgeui)

MGE_CAPTURE_TEST(TARGET hello_ui
                 RENDER_SYSTEMS opengl directx11 directx12 vulkan)
