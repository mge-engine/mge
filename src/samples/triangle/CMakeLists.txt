# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(triangle
               triangle.cpp)
TARGET_LINK_LIBRARIES(triangle mgecore mgeapplication mgeinput mgegraphics)
