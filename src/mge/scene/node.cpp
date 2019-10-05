#include "mge/scene/node.hpp"
#include "mge/scene/scene.hpp"

namespace mge {

    node::node()
        :m_node_entity(nil_entity)
        ,m_scene_entity(nil_entity)
    {}

    node::~node()
    {}

    void
    node::destroy()
    {
        scene *sc = scene::by_entity(m_scene_entity);
        if (sc) {
            sc->destroy_node(m_node_entity);
        }
    }
}