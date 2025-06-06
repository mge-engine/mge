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

    struct up
    {
        mge::fvec3 value;
    };

    static mge::scene::world::component_registrar
        up_registrar([](mge::entity::registry& r) {
            MGE_DEBUG_TRACE(SCENE) << "Registering camera up component";
            r.component<up>().set(mge::fvec3(0.0f, 1.0f, 0.0f));
        });

    void camera::set_up(const mge::fvec3& u)
    {
        m_entity.set<mge::scene::up>({u});
    }

    const mge::fvec3& camera::up() const
    {
        auto u = m_entity.get<mge::scene::up>();
        if (!u) {
            MGE_THROW(mge::illegal_state)
                << "camera does not have an up component";
        }
        return u->value;
    }

    struct fov
    {
        float value; //!< The field of view value
    };

    static mge::scene::world::component_registrar
        fov_registrar([](mge::entity::registry& r) {
            MGE_DEBUG_TRACE(SCENE) << "Registering camera fov component";
            fov fov_default{45.0f};
            r.component<mge::scene::fov>().set(fov_default);
        });

    void camera::set_fov(float f)
    {
        m_entity.set<mge::scene::fov>({f});
    }

    float camera::fov() const
    {
        auto f = m_entity.get<mge::scene::fov>();
        if (!f) {
            MGE_THROW(mge::illegal_state)
                << "camera does not have a fov component";
        }
        return f->value;
    }

    struct aspect
    {
        float value; //!< The aspect ratio value
    };

    static mge::scene::world::component_registrar
        aspect_registrar([](mge::entity::registry& r) {
            MGE_DEBUG_TRACE(SCENE) << "Registering camera aspect component";
            aspect aspect_default{1.0f};
            r.component<mge::scene::aspect>().set(aspect_default);
        });

    void camera::set_aspect(float a)
    {
        m_entity.set<mge::scene::aspect>({a});
    }

    float camera::aspect() const
    {
        auto a = m_entity.get<mge::scene::aspect>();
        if (!a) {
            MGE_THROW(mge::illegal_state)
                << "camera does not have an aspect component";
        }
        return a->value;
    }

} // namespace mge::scene