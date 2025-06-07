// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/scene.hpp"
#include "mge/core/trace.hpp"
#include "mge/scene/world.hpp"

namespace mge::scene {

    scene::scene(world& w)
        : m_scene_entity(w.world_registry())
    {
        using mge::entity::entity;
        m_scene_entity.add(entity::child_of, w.world_entity());
        m_scene_entity.add(entity::is_a, w.scene_type());
    }

    scene::scene(world& w, const char* name)
        : m_scene_entity(w.world_registry(), name)
    {
        using mge::entity::entity;
        m_scene_entity.add(entity::child_of, w.world_entity());
        m_scene_entity.add(entity::is_a, w.scene_type());
    }

    scene::scene(const mge::entity::entity& e) noexcept
        : m_scene_entity(e)
    {}

    scene::~scene() {}

    void scene::destroy()
    {
        if (m_scene_entity.is_valid()) {
            m_scene_entity.destroy();
        }
    }

    const char* scene::name() const
    {
        return m_scene_entity.name();
    }
} // namespace mge::scene