// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/entity.hpp"
#include "mge/scene/dllexport.hpp"
#include "mge/scene/scene_fwd.hpp"

namespace mge::scene {

    class MGESCENE_EXPORT camera : public mge::entity::entity
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

        void destroy();

        std::string_view name() const;

        const mge::entity::entity& camera_entity() const noexcept
        {
            return m_camera_entity;
        }

    private:
        mge::entity::entity m_camera_entity; //!< The camera entity
    };

} // namespace mge::scene
