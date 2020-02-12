# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_LIBRARY(mgeshader
    SHARED
    location.cpp
    program.cpp
    program_element.cpp)

TARGET_ADD_DEFINE(mgeshader BUILD_MGE_SHADER)
TARGET_LINK_LIBRARIES(mgeshader mgecore)

#ADD_SUBDIRECTORY(test)
