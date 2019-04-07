# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(blackscreen
               blackscreen.cpp)
TARGET_LINK_LIBRARIES(blackscreen mgecore mgeapplication mgeinput mgegraphics)
