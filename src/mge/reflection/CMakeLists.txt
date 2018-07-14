# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mgereflection
            SHARED
            module.cpp
            module_definition.cpp)

TARGET_ADD_DEFINE(mgereflection BUILD_MGE_REFLECTION)
TARGET_LINK_LIBRARIES(mgereflection mgecore)

ADD_SUBDIRECTORY(test)

