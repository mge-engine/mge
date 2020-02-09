# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_LIBRARY(mgeshader
    SHARED
    program.cpp)

TARGET_ADD_DEFINE(mgeshader BUILD_MGE_SHADER)
TARGET_LINK_LIBRARIES(mgeshader mgecore)

#ADD_SUBDIRECTORY(test)
