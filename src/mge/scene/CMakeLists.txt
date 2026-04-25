# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgescene_sources
    scene.cpp
)

SET(mgescene_headers
    dllexport.hpp
    scene_fwd.hpp
    scene.hpp
)

MGE_LIBRARY(
    TARGET  mgescene
    TYPE    SHARED
    SOURCES
    ${mgescene_sources}
    ${mgescene_headers}
)

TARGET_LINK_LIBRARIES(mgescene mgegraphics mgecore)
ADD_SUBDIRECTORY(test)
