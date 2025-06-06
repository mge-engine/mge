# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mgemath_sources
    dummy.cpp)

SET(mgemath_headers
    glm.hpp
    vec2.hpp
    vec3.hpp
    vec4.hpp
    quaternion.hpp)

MGE_LIBRARY(
    TARGET mgemath
    TYPE SHARED
    SOURCES
    ${mgemath_sources}
    ${mgemath_headers})

TARGET_LINK_LIBRARIES(mgemath mgecore)
ADD_SUBDIRECTORY(test)