// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/entity.hpp"
#include "mge/scene/dllexport.hpp"
#include "mge/scene/world.hpp"

namespace mge::scene {

    /**
     * Scene entity. A scene is a collection of presentation entities that
     * will render to the screen.
     */
    class MGESCENE_EXPORT scene
    {
    public:
        explicit scene(world& w);
        scene(world& w, const char* name);
        explicit scene(const mge::entity::entity& e) noexcept;

        scene(const scene&) = default;
        scene& operator=(const scene&) = default;
        scene(scene&&) noexcept = default;
        scene& operator=(scene&&) noexcept = default;
        ~scene();

        void destroy();

        const char* name() const;

        const mge::entity::entity& scene_entity() const noexcept
        {
            return m_scene_entity;
        }

    private:
        mge::entity::entity m_scene_entity; //!< The scene entity
    };

} // namespace mge::scene
