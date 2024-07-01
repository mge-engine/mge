# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgeentity_sources
   registry.cpp
   entity.cpp
)

SET(mgeentity_headers
    dllexport.hpp
    registry.hpp
    entity.hpp
    component.hpp
)


MGE_LIBRARY(
    TARGET  mgeentity
    TYPE    SHARED
    SOURCES
    ${mgeentity_sources}
    ${mgeentity_headers}
)
TARGET_LINK_LIBRARIES(mgeentity mgecore boost-all)
ADD_SUBDIRECTORY(test)
