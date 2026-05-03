// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/scene/dllexport.hpp"
#include "mge/scene/entity.hpp"

#include <flecs.h>
#include <string_view>

namespace mge {

    class node;

    /**
     * @brief Scene, master container for entities and systems.
     */
    class MGESCENE_EXPORT scene
    {
    public:
        scene();
        ~scene();

        scene(const scene&) = delete;
        scene& operator=(const scene&) = delete;
        scene(scene&&) = default;
        scene& operator=(scene&&) = default;

        entity create_entity();
        entity create_entity(std::string_view name);

        node create_node();
        node create_node(std::string_view name);

        template <typename... Components>
        entity create_entity(Components&&... components)
        {
            entity e = create_entity();
            (e.set(std::forward<Components>(components)), ...);
            return e;
        }

        template <typename... Components>
        entity create_entity(std::string_view name, Components&&... components)
        {
            entity e = create_entity(name);
            (e.set(std::forward<Components>(components)), ...);
            return e;
        }

        void update(float delta_time = 0.0f);

        flecs::world&       world() noexcept;
        const flecs::world& world() const noexcept;

        template <typename T> scene& set(const T& value)
        {
            m_scene_entity.set(value);
            return *this;
        }

        template <typename T> const T* get() const
        {
            return m_scene_entity.get<T>();
        }

        template <typename T> bool has() const
        {
            return m_scene_entity.has<T>();
        }

    private:
        flecs::world m_world;
        entity       m_scene_entity;
    };

} // namespace mge
