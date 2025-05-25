// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "world.hpp"

namespace mge::scene {

    world::world()
    {
        m_world_entity = mge::entity::entity(m_registry);
        m_scene_type = mge::entity::entity(m_registry, "scene");
        m_actor_type = mge::entity::entity(m_registry, "actor");
        m_camera_type = mge::entity::entity(m_registry, "camera");
        m_light_type = mge::entity::entity(m_registry, "light");
        m_geometry_type = mge::entity::entity(m_registry, "geometry");
    }

    world::~world()
    {
        m_geometry_type.destroy();
        m_light_type.destroy();
        m_camera_type.destroy();
        m_actor_type.destroy();
        m_scene_type.destroy();
        m_world_entity.destroy();
    }

} // namespace mge::scene
