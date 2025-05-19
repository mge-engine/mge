# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgeentity_sources
    world.cpp
)

SET(mgeentity_headers
    dllexport.hpp
    world.hpp
)

MGE_LIBRARY(
    TARGET  mgeentity
    TYPE    SHARED
    SOURCES
    ${mgeentity_sources}
    ${mgeentity_headers}
)

TARGET_LINK_LIBRARIES(mgeentity 
    mgemath 
    mgecore 
    mgeinput 
    boost-all 
    $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
)
ADD_SUBDIRECTORY(test)
