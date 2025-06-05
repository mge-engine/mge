// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/scene/camera.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/scene/world.hpp"

namespace mge {
    MGE_USE_TRACE(SCENE);
}

namespace mge::scene {
    camera::camera(mge::scene::world& w)
        : mge::scene::spatial(w)
    {
        m_entity.add(entity::is_a, w.camera_type());
    }

    camera::camera(mge::scene::world& w, const char* name)
        : mge::scene::spatial(w, name)
    {
        using mge::entity::entity;
        m_entity.add(entity::is_a, w.camera_type());
    }

    camera::camera(const mge::entity::entity& e) noexcept
        : spatial(e)
    {}

    camera::~camera() {}

    struct look_at
    {
        mge::fvec3 value;
    };

    static mge::scene::world::component_registrar
        look_at_registrar([](mge::entity::registry& r) {
            MGE_DEBUG_TRACE(SCENE) << "Registering camera look_at component";
            r.component<look_at>();
        });

    void camera::set_look_at(const mge::fvec3& la)
    {
        m_entity.set<mge::scene::look_at>({la});
    }

    const mge::fvec3& camera::look_at() const
    {
        auto la = m_entity.get<mge::scene::look_at>();
        if (!la) {
            MGE_THROW(mge::illegal_state)
                << "camera does not have a look_at component";
        }
        return la->value;
    }

} // namespace mge::scene