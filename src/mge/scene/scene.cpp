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
        // let's not remove the scene entity itself to 
        // prevent recycling
        scene_registry.remove<scene *>(m_scene_entity);
    }

    void
    scene::destroy_node(mge::entity node_entity)
    {
        m_registry.destroy(node_entity);
    }

    scene *
    scene::by_entity(mge::entity scene_entity)
    {
        auto scenepp = scene_registry.try_get<scene *>(scene_entity);
        if (scenepp) {
            return *scenepp;
        } else {
            return nullptr;
        }
    }


}  