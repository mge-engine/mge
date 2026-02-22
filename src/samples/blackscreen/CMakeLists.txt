# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(blackscreen
               blackscreen.cpp)

TARGET_LINK_LIBRARIES(blackscreen
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics)

MGE_CAPTURE_TEST(TARGET blackscreen)