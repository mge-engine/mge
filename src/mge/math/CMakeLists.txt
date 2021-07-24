# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
SET(mgemath_sources
    vec2.cpp)

SET(mgemath_headers
    glm.hpp
    vec2.hpp)

MGE_LIBRARY(
    TARGET mgemath
    TYPE SHARED
    SOURCES
    ${mgemath_sources}
    ${mgemath_headers})

TARGET_LINK_LIBRARIES(mgemath mgecore)
