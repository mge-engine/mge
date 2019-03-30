# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_LIBRARY(mgescene
    SHARED
    scene.cpp)

TARGET_ADD_DEFINE(mgescene BUILD_MGE_GRAPHICS)
TARGET_LINK_LIBRARIES(mgescene mgecore mgeinput mgeapplication)

#ADD_SUBDIRECTORY(test)
