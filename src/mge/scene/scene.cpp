// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/scene.hpp"
#include "mge/core/trace.hpp"
#include "mge/scene/world.hpp"

namespace mge {
    MGE_DEFINE_TRACE(SCENE);
}

namespace mge::scene {
    scene::scene(world& w)
        : m_scene_entity(w.m_registry)
    {
        m_scene_entity.add(flecs::ChildOf, w.m_world_entity);
    }

    scene::~scene()
    {
        if (m_scene_entity.is_valid()) {
            m_scene_entity.destroy();
        }
    }
} // namespace mge::scene