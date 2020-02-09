# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_LIBRARY(mgescene
    SHARED
    node.cpp
    scene.cpp
    renderer.cpp)

TARGET_ADD_DEFINE(mgescene BUILD_MGE_SCENE)
TARGET_LINK_LIBRARIES(mgescene mgecore mgeinput mgeentity mgeapplication)

ADD_SUBDIRECTORY(test)
