// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/world.hpp"
#include "mge/scene/scene.hpp"

namespace mge {

    world::world() = default;

    world::~world() = default;

    scene world::create_scene()
    {
        return scene(*this, m_world.entity());
    }

    scene world::create_scene(std::string_view name)
    {
        return scene(*this, m_world.entity(name.data()));
    }

    entity world::create_entity()
    {
        return entity(m_world.entity());
    }

    entity world::create_entity(std::string_view name)
    {
        return entity(m_world.entity(name.data()));
    }

    void world::update(float delta_time)
    {
        m_world.progress(delta_time);
    }

    flecs::world& world::flecs_world() noexcept
    {
        return m_world;
    }

    const flecs::world& world::flecs_world() const noexcept
    {
        return m_world;
    }

} // namespace mge
