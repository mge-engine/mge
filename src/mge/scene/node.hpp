#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/entity/entity.hpp"
#include "mge/scene/scene_fwd.hpp"

namespace mge {

    /**
     * A node is an element of the scene.
     */
    class MGE_SCENE_EXPORT node 
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