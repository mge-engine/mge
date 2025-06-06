// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/entity/entity.hpp"
#include "mge/scene/dllexport.hpp"
#include "mge/scene/scene_fwd.hpp"

#include "mge/math/quaternion.hpp"
#include "mge/math/vec.hpp"

#include <string_view>

namespace mge::scene {

    class MGESCENE_EXPORT spatial : public mge::entity::entity
    {
    public:
        explicit spatial(mge::scene::world& w);
        spatial(mge::scene::world& w, const char* name);
        explicit spatial(const mge::entity::entity& e) noexcept;

        spatial(const spatial&) = default;
        spatial& operator=(const spatial&) = default;
        spatial(spatial&&) noexcept = default;
        spatial& operator=(spatial&&) noexcept = default;
        ~spatial();

        void destroy();

        std::string_view name() const;

        const mge::entity::entity& spatial_entity() const noexcept
        {
            return m_entity;
        }

        void              set_position(const mge::fvec3& p);
        const mge::fvec3& position() const;

        void              set_rotation(const mge::fquat& r);
        const mge::fquat& rotation() const;

        void              set_scale(const mge::fvec3& s);
        const mge::fvec3& scale() const;

    protected:
        mge::entity::entity m_entity; //!< The entity
    };

} // namespace mge::scene
