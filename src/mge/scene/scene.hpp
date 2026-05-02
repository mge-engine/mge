// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/scene/dllexport.hpp"

#include <flecs.h>
#include <string_view>

namespace mge {

    /**
     * @brief Scene, master container for entities and systems.
     */
    class MGESCENE_EXPORT scene
    {
    public:
        /**
         * @brief Wrapper around a flecs entity belonging to this scene.
         */
        class MGESCENE_EXPORT entity
        {
        public:
            entity() = default;
            entity(const entity&) = default;
            entity(entity&&) = default;
            entity& operator=(const entity&) = default;
            entity& operator=(entity&&) = default;
            ~entity() = default;

            flecs::entity_t id() const noexcept;
            bool            valid() const noexcept;

            template <typename T> entity& set(const T& value)
            {
                m_entity.set<T>(value);
                return *this;
            }

            template <typename T> const T* get() const
            {
                return m_entity.get<T>();
            }

            template <typename T> bool has() const
            {
                return m_entity.has<T>();
            }

        private:
            friend class scene;
            explicit entity(flecs::entity e);
            flecs::entity m_entity;
        };

        scene();
        ~scene();

        scene(const scene&) = delete;
        scene& operator=(const scene&) = delete;
        scene(scene&&) = default;
        scene& operator=(scene&&) = default;

        entity create_entity();
        entity create_entity(std::string_view name);

        void update(float delta_time = 0.0f);

        flecs::world&       world() noexcept;
        const flecs::world& world() const noexcept;

    private:
        flecs::world m_world;
    };

} // namespace mge
