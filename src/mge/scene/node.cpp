// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/node.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace mge {

    node::node(entity e)
        : entity(std::move(e))
    {
        if (!has<mge::node::tag>()) {
            set(mge::node::tag{});
        }
        if (!has<mge::node_position>()) {
            set(mge::node_position{});
        }
        if (!has<mge::node_rotation>()) {
            set(mge::node_rotation{});
        }
        if (!has<mge::node_scale>()) {
            set(mge::node_scale{});
        }
        if (!has<mge::node_transform>()) {
            set(mge::node_transform{});
        }
    }

    const fvec3& node::position() const
    {
        return get<mge::node_position>()->value;
    }

    void node::set_position(const fvec3& pos)
    {
        set(mge::node_position{pos});
        recompute_transform();
    }

    const fquat& node::rotation() const
    {
        return get<mge::node_rotation>()->value;
    }

    void node::set_rotation(const fquat& rot)
    {
        set(mge::node_rotation{rot});
        recompute_transform();
    }

    const fvec3& node::scale() const
    {
        return get<mge::node_scale>()->value;
    }

    void node::set_scale(const fvec3& s)
    {
        set(mge::node_scale{s});
        recompute_transform();
    }

    const fmat4& node::transform() const
    {
        return get<mge::node_transform>()->matrix;
    }

    void node::recompute_transform()
    {
        fmat4 m = glm::translate(fmat4{1.f}, position()) *
                  glm::mat4_cast(rotation()) * glm::scale(fmat4{1.f}, scale());
        set(mge::node_transform{m});
    }

} // namespace mge
