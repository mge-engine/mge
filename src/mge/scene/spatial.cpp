// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/spatial.hpp"
#include "mge/core/trace.hpp"
#include "mge/math/vec.hpp"
#include "mge/scene/world.hpp"

namespace mge {
    MGE_USE_TRACE(SCENE);
}

namespace mge::scene {

    spatial::spatial(mge::scene::world& w)
        : m_entity(w.world_registry())
    {
        using mge::entity::entity;
        m_entity.add(entity::child_of, w.world_entity());
        m_entity.add(entity::is_a, w.spatial_type());
    }

    spatial::spatial(mge::scene::world& w, const char* name)
        : m_entity(w.world_registry(), name)
    {
        using mge::entity::entity;
        m_entity.add(entity::child_of, w.world_entity());
        m_entity.add(entity::is_a, w.spatial_type());
    }

    spatial::spatial(const mge::entity::entity& e) noexcept
        : m_entity(e)
    {}

    spatial::~spatial() {}

    void spatial::destroy()
    {
        if (m_entity.is_valid()) {
            m_entity.destroy();
        }
    }

    std::string_view spatial::name() const
    {
        auto flecsstr = m_entity.name();
        return std::string_view(flecsstr.c_str(),
                                flecsstr.c_str() + flecsstr.length());
    }

    struct position
    {
        mge::fvec3 value; //!< The position value
    };

    static mge::scene::world::component_registrar
        position_registrar([](mge::entity::registry& r) {
            MGE_DEBUG_TRACE(SCENE) << "Registering position component";
            r.component<position>();
        });

} // namespace mge::scene