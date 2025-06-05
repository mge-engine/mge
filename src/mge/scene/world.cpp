// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "world.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(SCENE);
}

namespace mge::scene {

    class component_registrars
    {
    public:
        void add(world::component_registrar* registrar)
        {
            m_registrars.push_back(registrar);
        }

        void execute(mge::entity::registry& r) const
        {
            for (auto& registrar : m_registrars) {
                registrar->execute(r);
            }
        }

    private:
        std::vector<world::component_registrar*> m_registrars;
    };

    static mge::singleton<component_registrars> s_component_registrars;

    world::world()
    {
        m_world_entity = mge::entity::entity(m_registry);
        m_scene_type = mge::entity::entity(m_registry, "scene");
        m_actor_type = mge::entity::entity(m_registry, "actor");
        m_camera_type = mge::entity::entity(m_registry, "camera");
        m_light_type = mge::entity::entity(m_registry, "light");
        m_geometry_type = mge::entity::entity(m_registry, "geometry");
        m_spatial_type = mge::entity::entity(m_registry, "spatial");

        s_component_registrars->execute(m_registry);
    }

    world::~world()
    {
        m_geometry_type.destroy();
        m_light_type.destroy();
        m_camera_type.destroy();
        m_actor_type.destroy();
        m_scene_type.destroy();
        m_world_entity.destroy();
        m_spatial_type.destroy();
    }

    world::component_registrar::component_registrar(
        std::function<void(mge::entity::registry&)>&& f)
        : m_function(std::move(f))
    {
        s_component_registrars->add(this);
    }

} // namespace mge::scene
