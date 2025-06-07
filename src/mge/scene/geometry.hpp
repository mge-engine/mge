// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/scene/dllexport.hpp"
#include "mge/scene/scene_fwd.hpp"
#include "mge/scene/spatial.hpp"

#include "mge/graphics/mesh.hpp"

namespace mge::scene {

    class MGESCENE_EXPORT geometry : public mge::scene::spatial
    {
    public:
        explicit geometry(mge::scene::world& w);
        geometry(mge::scene::world& w, const char* name);
        explicit geometry(const mge::entity::entity& e) noexcept;

        geometry(const geometry&) = default;
        geometry& operator=(const geometry&) = default;
        geometry(geometry&&) noexcept = default;
        geometry& operator=(geometry&&) noexcept = default;
        ~geometry();

        const mge::entity::entity& geometry_entity() const noexcept
        {
            return m_entity;
        }

        void                 set_mesh(const mge::mesh_ref& mesh);
        const mge::mesh_ref& mesh() const;
    };
} // namespace mge::scene