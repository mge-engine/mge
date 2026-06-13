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
     * @brief Handle to a scene object.
     *
     * An entity identifies one object in a scene by its numeric id. Entity
     * instances are lightweight handles. They can be copied and moved without
     * copying the components stored on the scene object.
     *
     * Components can be attached with set(), queried with get() and tested
     * with has(). Derived scene wrapper types can use is() to check whether
     * the entity carries their tag component.
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

        /**
         * Get the entity id. The id is a non-zero integer that uniquely
         * identifies the entity in the scene.
         * @return entity id
         */
        id_type id() const noexcept;

        /**
         * Check if the entity is valid. An entity is valid if it has a non-zero
         * id.
         * @return true if the entity is valid, false otherwise
         */
        bool valid() const noexcept;

        /**
         * Get the name of the entity. The name is an optional string that can
         * be assigned to an entity. It is not guaranteed to be unique. If the
         * entity does not have a name, an empty string is returned.
         * @return entity name
         */
        std::string_view name() const noexcept;

        /**
         * Check if the entity is valid.
         * @return true if the entity is valid, false otherwise
         */
        inline operator bool() const noexcept
        {
            return valid();
        }

        /**
         * Set a component on the entity. If the component already exists, it is
         * updated with the new value. If the component does not exist, it is
         * created and initialized with the provided value.
         * @tparam T component type
         * @param value component value
         * @return reference to the entity
         */
        template <typename T> entity& set(const T& value)
        {
            m_entity.set<T>(value);
            return *this;
        }

        /**
         * Set a component on the entity. If the component already exists, it is
         * updated with the new value. If the component does not exist, it is
         * created and initialized with the provided value.
         * @tparam T component type
         * @param value component value
         * @return reference to the entity
         */
        template <typename T> entity& set(T&& value)
        {
            m_entity.set<T>(std::forward<T>(value));
            return *this;
        }

        /**
         * Get a component from the entity. If the entity does not have the
         * component, nullptr is returned.
         * @tparam T component type
         * @return pointer to component value or nullptr if not found
         */
        template <typename T>
        const T* get() const noexcept(noexcept(m_entity.try_get<T>()))
        {
            return m_entity.try_get<T>();
        }

        /**
         * Check if the entity has a component.
         * @tparam T component type
         * @return true if the entity has the component, false otherwise
         */
        template <typename T> bool has() const
        {
            return m_entity.has<T>();
        }

        /**
         * Check if the entity has a tag component. A tag component is a
         * component that is used to identify a specific type of entity. It does
         * not have any data associated with it. The tag component must be
         * defined as a nested type named `tag` in the derived scene wrapper
         * type.
         * @tparam T derived scene wrapper type
         * @return true if the entity has the tag component, false otherwise
         */
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
