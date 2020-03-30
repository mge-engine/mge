// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/properties.hpp"

namespace mge {
    class MGE_CORE_EXPORT property_object : mge::noncopyable
    {
    public:
        property_object()  = default;
        ~property_object() = default;

        std::string get_property(const std::string &key) const;
        std::string get_property(const std::string &key,
                                 const std::string &default_value) const;
        bool        exists_property(const std::string &key) const;
        void        erase_property(const std::string &key);
        void        clear_properties();

        template <typename T> T get_property(const std::string &key) const
        {
            return m_properties.get<T>(key);
        }

        template <typename T, typename U>
        T get_property(const std::string &key, const U &default_value) const
        {
            return m_properties.get<T, U>(key, default_value);
        }

    private:
        properties m_properties;
    };
} // namespace mge
