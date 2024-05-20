# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgeentity_sources
   registry.cpp
)

SET(mgeentity_headers
    dllexport.hpp
    registry.hpp
)


MGE_LIBRARY(
    TARGET  mgeentity
    TYPE    SHARED
    SOURCES
    ${mgeentity_sources}
    ${mgeentity_headers}
)
TARGET_LINK_LIBRARIES(mgecore boost-all)
ADD_SUBDIRECTORY(test)
