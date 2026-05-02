// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/scene.hpp"

namespace mge {

    // scene::entity

    scene::entity::entity(flecs::entity e)
        : m_entity(e)
    {}

    flecs::entity_t scene::entity::id() const noexcept
    {
        return m_entity.id();
    }

    bool scene::entity::valid() const noexcept
    {
        return m_entity.is_valid();
    }

    // scene

    scene::scene() = default;
    scene::~scene() = default;

    scene::entity scene::create_entity()
    {
        return entity(m_world.entity());
    }

    scene::entity scene::create_entity(std::string_view name)
    {
        return entity(m_world.entity(name.data()));
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
