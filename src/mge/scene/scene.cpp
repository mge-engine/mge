// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/scene.hpp"
#include "mge/scene/node.hpp"
#include "mge/scene/world.hpp"

#include <memory>
#include <utility>

namespace mge {

    // scene

    scene::scene()
        : scene(std::make_shared<mge::world>())
    {}

    scene::scene(mge::world& world)
        : scene(world, world.flecs_world().entity())
    {}

    scene::scene(std::shared_ptr<mge::world> world)
        : m_owned_world(std::move(world))
        , m_world(m_owned_world.get())
        , m_scene_entity(m_world->flecs_world().entity("_scene"))
    {}

    scene::scene(mge::world& world, flecs::entity scene_entity)
        : m_world(&world)
        , m_scene_entity(scene_entity)
    {}

    scene::~scene() = default;

    entity scene::create_entity()
    {
        return m_world->create_entity();
    }

    entity scene::create_entity(std::string_view name)
    {
        return m_world->create_entity(name);
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
        m_world->update(delta_time);
    }

    flecs::world& scene::world() noexcept
    {
        return m_world->flecs_world();
    }

    const flecs::world& scene::world() const noexcept
    {
        return m_world->flecs_world();
    }

} // namespace mge
