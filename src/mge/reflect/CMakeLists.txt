# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
SET(mgereflect_sources
    type_details.cpp)

SET(mgereflect_headers
    type_details.hpp)

MGE_LIBRARY(
    TARGET mgereflect
    TYPE SHARED
    SOURCES
    ${mgereflect_sources}
    ${mgereflect_headers})

TARGET_LINK_LIBRARIES(mgereflect mgecore)
ADD_SUBDIRECTORY(test)