#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/entity/entity.hpp"
#include "mge/scene/scene_fwd.hpp"

namespace mge {

    /**
     * Node object. Note a @c node is only a view on the actual
     * node stored in a scene.
     */
    class MGE_SCENE_EXPORT node 
    {
    public:
        node();
        ~node();

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
        mge::entity m_node_entity;
        mge::entity m_scene_entity;
    };
}