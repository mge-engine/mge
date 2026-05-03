// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/scene/dllexport.hpp"

#include <flecs.h>
#include <string_view>
#include <type_traits>

namespace mge {

    class scene;

    /**
     * @brief Entity, a scene object identified by a unique id.
     */
    class MGESCENE_EXPORT entity
    {
    protected:
        struct tag
        {};

    public:
        using id_type = flecs::entity_t;

        entity() = default;
        entity(const entity&) = default;
        entity(entity&&) = default;
        entity& operator=(const entity&) = default;
        entity& operator=(entity&&) = default;
        ~entity() = default;

        id_type          id() const noexcept;
        bool             valid() const noexcept;
        std::string_view name() const noexcept;

        inline operator bool() const noexcept
        {
            return valid();
        }

        template <typename T> entity& set(const T& value)
        {
            m_entity.set<T>(value);
            return *this;
        }

        template <typename T> const T* get() const
        {
            return m_entity.try_get<T>();
        }

        template <typename T> bool has() const
        {
            return m_entity.has<T>();
        }

        template <typename T>
            requires requires { typename T::tag; } &&
                     std::is_base_of_v<entity::tag, typename T::tag>
        bool is() const
        {
            return m_entity.has<typename T::tag>();
        }

    private:
        friend class scene;
        friend class world;
        explicit entity(flecs::entity e);
        flecs::entity m_entity;
    };

} // namespace mge
