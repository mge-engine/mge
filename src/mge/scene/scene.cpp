// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/scene.hpp"
#include "mge/scene/node.hpp"

namespace mge {

    // scene

    scene::scene()
        : m_scene_entity(m_world.entity("_scene"))
    {}

    scene::~scene() = default;

    entity scene::create_entity()
    {
        return entity(m_world.entity());
    }

    entity scene::create_entity(std::string_view name)
    {
        return entity(m_world.entity(name.data()));
    }

    node scene::create_node()
    {
        return node(create_entity());
    }

    node scene::create_node(std::string_view name)
    {
        return node(create_entity(name));
    }

    void scene::update(float delta_time)
    {
        m_world.progress(delta_time);
    }

    flecs::world& scene::world() noexcept
    {
        return m_world;
    }

    const flecs::world& scene::world() const noexcept
    {
        return m_world;
    }

} // namespace mge
