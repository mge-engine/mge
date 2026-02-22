# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(depth_test
               depth_test.cpp)

TARGET_LINK_LIBRARIES(depth_test
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics
    mgeasset)

MGE_CAPTURE_TEST(TARGET depth_test)
