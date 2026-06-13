// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/scene/dllexport.hpp"
#include "mge/scene/entity.hpp"

#include <flecs.h>
#include <string_view>

namespace mge {

    class scene;

    /**
     * @brief World containing scenes and their entities.
     */
    class MGESCENE_EXPORT world
    {
    public:
        world();
        ~world();

        world(const world&) = delete;
        world& operator=(const world&) = delete;
        world(world&&) = delete;
        world& operator=(world&&) = delete;

        scene create_scene();
        scene create_scene(std::string_view name);

        entity create_entity();
        entity create_entity(std::string_view name);

        void update(float delta_time = 0.0f);

        flecs::world&       flecs_world() noexcept;
        const flecs::world& flecs_world() const noexcept;

    private:
        flecs::world m_world;
    };

} // namespace mge
