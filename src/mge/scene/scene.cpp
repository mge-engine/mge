// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/scene.hpp"
namespace mge {

    static mge::entity_registry scene_registry;

    scene::scene()
    {
        m_scene_entity = scene_registry.create();
        scene_registry.assign<scene *>(m_scene_entity, this);
    }

    scene::~scene()
    {
        scene_registry.destroy(m_scene_entity);
    }

    void
    scene::destroy_node(mge::entity node_entity)
    {
        m_registry.destroy(node_entity);
    }

    scene *
    scene::by_entity(mge::entity scene_entity)
    {
        if (scene_registry.valid(scene_entity)) {
            auto scenepp = scene_registry.try_get<scene *>(scene_entity);
            if (scenepp) {
                return *scenepp;
            } 
        }
        return nullptr;
    }

    mge::entity_registry&
    scene::registry() noexcept
    {
        return scene_registry;
    }

    node
    scene::create_node()
    {
        auto node_entity = m_registry.create();
        return node(m_scene_entity, node_entity);
    }

    bool
    scene::is_node_valid(const node& n) const
    {
        return m_scene_entity == n.m_scene_entity && m_registry.valid(n.m_node_entity);
    }


}  
