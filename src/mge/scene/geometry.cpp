// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/geometry.hpp"
#include "mge/core/trace.hpp"
#include "mge/scene/world.hpp"

namespace mge {
    MGE_USE_TRACE(SCENE);
}

namespace mge::scene {

    geometry::geometry(mge::scene::world& w)
        : spatial(w)
    {}

    geometry::geometry(mge::scene::world& w, const char* name)
        : spatial(w, name)
    {}

    geometry::geometry(const mge::entity::entity& e) noexcept
        : spatial(e)
    {}

    geometry::~geometry() {}

    struct geometry_mesh
    {
        mge::mesh_ref value;
    };

    static mge::scene::world::component_registrar
        geometry_mesh_registrar([](mge::entity::registry& r) {
            MGE_DEBUG_TRACE(SCENE) << "Registering geometry mesh component";
            r.component<geometry_mesh>();
        });

    void geometry::set_mesh(const mge::mesh_ref& m)
    {
        m_entity.set<mge::scene::geometry_mesh>({m});
    }

    const mge::mesh_ref& geometry::mesh() const
    {
        auto m = m_entity.get<mge::scene::geometry_mesh>();
        if (!m) {
            MGE_THROW(mge::illegal_state)
                << "geometry does not have a mesh component";
        }
        return m->value;
    }

} // namespace mge::scene