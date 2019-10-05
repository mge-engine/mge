// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/scene/node.hpp"
#include "mge/entity/entity_registry.hpp"

namespace mge {

    /**
     * A scene combines all objects displayed and interacting
     * with each other (and can be interacted with).
     */
    class MGE_SCENE_EXPORT scene
        : mge::noncopyable
    {
    public:
        /**
         * Create empty scene.
         */
        scene();

        /**
         * Destroy scene and all nodes.
         */
        ~scene();

        /**
         * Retrieve the scene by lookup of the scene entity.
         * 
         * @param scene_entity scene entity
         * @return pointer to scene if entity is valid
         */
        static scene* by_entity(mge::entity scene_entity);
        
        /**
         * Destroys a node by entity.
         * 
         * @param node_entity entity of node
         */
        void destroy_node(mge::entity node_entity);
    private:
        mge::entity_registry m_registry;
        mge::entity          m_scene_entity;
    };
}
