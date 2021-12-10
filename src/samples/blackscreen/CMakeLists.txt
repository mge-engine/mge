# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(blackscreen
               blackscreen.cpp)

TARGET_LINK_LIBRARIES(blackscreen
    mgecore
    mgegraphics
    mgeapplication
    mgeinput
    mgegraphics)
