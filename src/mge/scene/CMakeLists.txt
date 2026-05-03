# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgescene_sources
    entity.cpp
    node.cpp
    scene.cpp
    world.cpp
)

SET(mgescene_headers
    dllexport.hpp
    entity.hpp
    material.hpp
    material_type.hpp
    node.hpp
    pbr_material.hpp
    scene_fwd.hpp
    scene.hpp
    shader_material.hpp
    texture_material.hpp
    world.hpp
)

MGE_LIBRARY(
    TARGET  mgescene
    TYPE    SHARED
    SOURCES
    ${mgescene_sources}
    ${mgescene_headers}
)

TARGET_LINK_LIBRARIES(mgescene mgegraphics mgecore flecs::flecs)
ADD_SUBDIRECTORY(test)
