// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/node.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/scene/scene.hpp"

namespace mge {

    node::node() : m_scene_entity(nil_entity), m_node_entity(nil_entity) {}

    node::node(mge::entity scene_entity, mge::entity node_entity)
        : m_scene_entity(scene_entity), m_node_entity(node_entity)
    {}

    void node::destroy()
    {
        scene *sc = scene::by_entity(m_scene_entity);
        if (sc) {
            sc->destroy_node(m_node_entity);
        }
    }

    bool node::valid() const
    {
        if (m_node_entity == mge::nil_entity ||
            m_scene_entity == mge::nil_entity) {
            return false;
        }
        scene *sc = scene::by_entity(m_scene_entity);
        if (sc) {
            return sc->is_node_valid(*this);
        } else {
            return false;
        }
    }

    entity_registry &node::registry() const
    {
        scene *sc = scene::by_entity(m_scene_entity);
        if (sc) {
            return sc->node_registry();
        } else {
            MGE_THROW(mge::illegal_state)
                << "Invalid node state: invalid scene";
        }
    }
} // namespace mge
