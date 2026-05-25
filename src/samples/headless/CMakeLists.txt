# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(headless
               headless.cpp)

TARGET_LINK_LIBRARIES(headless
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics
    mgeasset)
