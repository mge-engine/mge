// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/scene/scene_fwd.hpp"
#include "mge/scene/spatial.hpp"

#include <string_view>

namespace mge::scene {

    class MGESCENE_EXPORT camera : public mge::scene::spatial
    {
    public:
        explicit camera(mge::scene::world& w);
        camera(mge::scene::world& w, const char* name);
        explicit camera(const mge::entity::entity& e) noexcept;

        camera(const camera&) = default;
        camera& operator=(const camera&) = default;
        camera(camera&&) noexcept = default;
        camera& operator=(camera&&) noexcept = default;
        ~camera();

        const mge::entity::entity& camera_entity() const noexcept
        {
            return m_entity;
        }

        void              set_look_at(const mge::fvec3& look_at);
        const mge::fvec3& look_at() const;

        void              set_up(const mge::fvec3& u);
        const mge::fvec3& up() const;

        void  set_fov(float f);
        float fov() const;

        void  set_aspect(float a);
        float aspect() const;
    };

} // namespace mge::scene
