# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

SET(mgeinput_sources
    key.cpp
    key_action.cpp
    mouse_action.cpp
    modifier.cpp
    input_handler.cpp
)

ADD_LIBRARY(mgeinput
            SHARED
            ${mgeinput_sources})

TARGET_ADD_DEFINE(mgeinput BUILD_MGE_INPUT)
TARGET_LINK_LIBRARIES(mgeinput mgecore)


ADD_SUBDIRECTORY(test)

