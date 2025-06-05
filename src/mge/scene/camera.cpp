// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/scene/camera.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/scene/world.hpp"

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

} // namespace mge::scene