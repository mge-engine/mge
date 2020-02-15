// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/entity/entity.hpp"
#include "mge/entity/entity_registry.hpp"
#include "mge/entity/component.hpp"
#include "mge/scene/scene_fwd.hpp"
#include "mge/scene/movable_entity.hpp"

namespace mge {

    /**
     * A node is an element of the scene.
     *
     * It is a movable entity in the way it can manage a custom
     * position, orientation and scale, resulting in an own
     * model transformation.
     *
     */
    class MGE_SCENE_EXPORT node 
            : public movable_entity<node>
    {
    public:
        /**
         * Create invalid node.
         */
        node();

        /**
         * Copy constructor.
         * 
         * @param n source node
         */
        node(const node& n)
            :m_scene_entity(n.m_scene_entity)
            ,m_node_entity(n.m_scene_entity)
        {}

        /**
         * Assignment.
         * 
         * @param n assigned node
         * @return @c *this
         */
        node& operator =(const node& n)
        {
            m_scene_entity = n.m_scene_entity;
            m_node_entity = n.m_node_entity;
            return *this;
        }

        /**
         * Destructor. Does not remove the node from the scene as the node
         * is only a view.
         */
        ~node() = default;

        /**
         * Destroys the node.
         */
        void destroy();

        /**
         * Checks that the node is valid.
         * 
         * @return @c true if the node is valid and belongs to a valid scene
         */
        bool valid() const;

        /**
         * Access node entity.
         * @return node entity
         */
        mge::entity entity() const noexcept { return m_node_entity; }

        /**
         * Access node entity registry.
         * @return node entity registry
         */
        mge::entity_registry& registry() const;

    private:
        friend class scene;

        /**
         * Constructor, used to create new node of scene.
         * 
         * @param scene_entity entity of scene
         * @param node_entity  entity of node
         */
        node(mge::entity scene_entity, mge::entity node_entity);

        mge::entity m_scene_entity;
        mge::entity m_node_entity;
    };
}
