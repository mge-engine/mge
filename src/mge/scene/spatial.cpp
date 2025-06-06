// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/scene/spatial.hpp"
#include "mge/core/stdexceptions.hpp"
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

    void spatial::set_position(const mge::fvec3& p)
    {
        m_entity.set<::mge::scene::position>({p});
    }

    const mge::fvec3& spatial::position() const
    {
        auto pos = m_entity.get<::mge::scene::position>();
        if (!pos) {
            MGE_THROW(mge::illegal_state)
                << "spatial does not have a position component";
        }
        return pos->value;
    }

    struct rotation
    {
        mge::fquat value; //!< The rotation value
    };

    static mge::scene::world::component_registrar
        position_registrar([](mge::entity::registry& r) {
            MGE_DEBUG_TRACE(SCENE) << "Registering position component";
            r.component<position>();
        });

    static mge::scene::world::component_registrar
        rotation_registrar([](mge::entity::registry& r) {
            MGE_DEBUG_TRACE(SCENE) << "Registering rotation component";
            r.component<rotation>().set(mge::identity_rotation);
        });

    void spatial::set_rotation(const mge::fquat& r)
    {
        m_entity.set<::mge::scene::rotation>({r});
    }

    const mge::fquat& spatial::rotation() const
    {
        auto rot = m_entity.get<::mge::scene::rotation>();
        if (!rot) {
            MGE_THROW(mge::illegal_state)
                << "spatial does not have a rotation component";
        }
        return rot->value;
    }

    struct scale
    {
        mge::fvec3 value;
    };

    static mge::scene::world::component_registrar
        scale_registrar([](mge::entity::registry& r) {
            MGE_DEBUG_TRACE(SCENE) << "Registering scale component";
            r.component<scale>().set(mge::fvec3(1.0f, 1.0f, 1.0f));
        });

    void spatial::set_scale(const mge::fvec3& s)
    {
        m_entity.set<::mge::scene::scale>({s});
    }

    const mge::fvec3& spatial::scale() const
    {
        auto scl = m_entity.get<::mge::scene::scale>();
        if (!scl) {
            MGE_THROW(mge::illegal_state)
                << "spatial does not have a scale component";
        }
        return scl->value;
    }

} // namespace mge::scene