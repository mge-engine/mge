# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

SET(mgemath_sources
    vec2.cpp
    vec3.cpp
    vec4.cpp
    aabox.cpp
    plane.cpp
    sphere.cpp
)
ADD_LIBRARY(mgemath
            SHARED
            ${mgemath_sources})

TARGET_ADD_DEFINE(mgemath BUILD_MGE_MATH)
TARGET_LINK_LIBRARIES(mgemath mgecore)

ADD_SUBDIRECTORY(test)
