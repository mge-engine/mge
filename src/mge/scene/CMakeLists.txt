# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgescene_sources
    scene.cpp
)

SET(mgeentity_headers
    dllexport.hpp
    scene.hpp
)

MGE_LIBRARY(
    TARGET  mgescene
    TYPE    SHARED
    SOURCES
    ${mgescene_sources}
    ${mgescene_headers}
)

TARGET_LINK_LIBRARIES(mgescene
    mgemath 
    mgecore 
    mgeinput 
    mgemath
    mgegraphics
    mgeasset
    mgeentity
    boost-all
)
ADD_SUBDIRECTORY(test)
