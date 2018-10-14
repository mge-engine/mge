# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mgescript
            SHARED
            script_engine.cpp
            script_context.cpp
            script_source.cpp)

TARGET_ADD_DEFINE(mgescript BUILD_MGE_SCRIPT)
TARGET_LINK_LIBRARIES(mgescript mgereflection mgecore)

#ADD_SUBDIRECTORY(test)

