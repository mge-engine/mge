# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(textured_cube
               textured_cube.cpp)
TARGET_LINK_LIBRARIES(textured_cube mgecore mgeapplication mgeinput mgegraphics)
