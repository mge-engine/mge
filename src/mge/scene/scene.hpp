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

        /**
         * Get the entity of this scene.
         * @return this scene's entity
         */
        mge::entity entity() const noexcept { return m_scene_entity; }

        /**
         * Create a new node.
         * 
         * @return node new node 
         */
        node create_node();

        /**
         * Return whether a node is valid.
         * 
         * @param n node to check
         * @return @c true if the node is valid
         */
        bool is_node_valid(const node& n) const;
    private:
        mge::entity_registry m_registry;
        mge::entity          m_scene_entity;
    };
}
