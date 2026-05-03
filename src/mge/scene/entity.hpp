// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/scene/dllexport.hpp"

#include <flecs.h>
#include <string_view>

namespace mge {

    class scene;

    /**
     * @brief Entity, a scene object identified by a unique id.
     */
    class MGESCENE_EXPORT entity
    {
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

    private:
        friend class scene;
        friend class world;
        explicit entity(flecs::entity e);
        flecs::entity m_entity;
    };

} // namespace mge
