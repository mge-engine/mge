// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/properties.hpp"

namespace mge {
    /**
     * Generic property handling object.
     */
    class MGE_CORE_EXPORT property_object : mge::noncopyable
    {
    public:
        /**
         * @brief Constructor.
         */
        property_object() = default;

        /**
         * @brief Destructor.
         */
        ~property_object() = default;

        /**
         * @brief Get a proprty by key.
         *
         * @param key key the property
         * @return property value
         */
        std::string get_property(const std::string &key) const;

        /**
         * @brief Get a property by key.
         *
         * @param key key the property
         * @param default_value value to return if property isn't set
         * @return property value
         */
        std::string get_property(const std::string &key,
                                 const std::string &default_value) const;

        /**
         * @brief Check that property exists.
         *
         * @param key property key
         * @return @c true if property exists
         */
        bool exists_property(const std::string &key) const;

        /**
         * @brief Erase a property.
         *
         * @param key property key
         */
        void erase_property(const std::string &key);

        /**
         * @brief Erases all properties.
         */
        void clear_properties();

        /**
         * @brief Get a property as type.
         *
         * @tparam T target type
         * @param key property key
         * @return property value, create by a lexical cast to @c T type
         */
        template <typename T> T get_property(const std::string &key) const
        {
            return m_properties.get<T>(key);
        }

        /**
         * @brief Get a property as type.
         *
         * @tparam T target type
         * @tparam U type of default value
         * @param key property key
         * @param default_value property default value
         * @return property value, create by a lexical cast to @c T type
         */
        template <typename T, typename U>
        T get_property(const std::string &key, const U &default_value) const
        {
            return m_properties.get<T, U>(key, default_value);
        }

    private:
        properties m_properties;
    };
} // namespace mge
