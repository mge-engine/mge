// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/type_name.hpp"
#include "mge/reflection/dllexport.hpp"

#include <iostream>
#include <tuple>
#include <typeindex>
#include <typeinfo>

namespace mge::reflection {

    /**
     * Represents a unique identifier for a type, including its const and
     * volatile qualifiers.
     */
    class MGEREFLECTION_EXPORT type_identifier
    {
    public:
        type_identifier(const std::type_info& type_info,
                        std::string_view      name,
                        bool                  is_const,
                        bool                  is_volatile,
                        bool                  is_reference) noexcept;
        ~type_identifier() = default;

        type_identifier(const type_identifier&) = default;
        type_identifier(type_identifier&&) noexcept = default;
        type_identifier& operator=(const type_identifier&) = default;
        type_identifier& operator=(type_identifier&&) noexcept = default;

        const std::type_index& type_index() const noexcept
        {
            return m_type_index;
        }

        bool is_const() const noexcept
        {
            return m_is_const;
        }

        bool is_volatile() const noexcept
        {
            return m_is_volatile;
        }

        bool is_reference() const noexcept
        {
            return m_is_reference;
        }

        std::string_view name() const noexcept
        {
            return m_name;
        }

        bool operator==(const type_identifier& other) const noexcept
        {
            return m_type_index == other.m_type_index &&
                   m_is_const == other.m_is_const &&
                   m_is_volatile == other.m_is_volatile &&
                   m_is_reference == other.m_is_reference;
        }

        bool operator!=(const type_identifier& other) const noexcept
        {
            return !(*this == other);
        }

        bool operator<(const type_identifier& other) const noexcept
        {
            return std::tie(m_type_index,
                            m_is_const,
                            m_is_volatile,
                            m_is_reference) < std::tie(other.m_type_index,
                                                       other.m_is_const,
                                                       other.m_is_volatile,
                                                       other.m_is_reference);
        }

        bool operator>(const type_identifier& other) const noexcept
        {
            return std::tie(m_type_index,
                            m_is_const,
                            m_is_volatile,
                            m_is_reference) > std::tie(other.m_type_index,
                                                       other.m_is_const,
                                                       other.m_is_volatile,
                                                       other.m_is_reference);
        }

        bool operator<=(const type_identifier& other) const noexcept
        {
            return !(*this > other);
        }

        bool operator>=(const type_identifier& other) const noexcept
        {
            return !(*this < other);
        }

    private:
        std::type_index  m_type_index;
        std::string_view m_name;
        bool             m_is_const;
        bool             m_is_volatile;
        bool             m_is_reference;
    };

    namespace {
        template <typename T> struct has_const
        {
            static constexpr bool value = std::is_const_v<T>;
        };
        template <typename T> struct has_const<const T>
        {
            static constexpr bool value = true;
        };
        template <typename T> struct has_const<const T&>
        {
            static constexpr bool value = true;
        };

        template <typename T> struct has_volatile
        {
            static constexpr bool value = std::is_volatile_v<T>;
        };
        template <typename T> struct has_volatile<volatile T>
        {
            static constexpr bool value = true;
        };
        template <typename T> struct has_volatile<volatile T&>
        {
            static constexpr bool value = true;
        };

        template <typename T> struct has_reference
        {
            static constexpr bool value = std::is_reference_v<T>;
        };
    } // namespace

    template <typename T> inline type_identifier make_type_identifier() noexcept
    {
        using base_type = std::remove_cv_t<std::remove_reference_t<T>>;
        return type_identifier(typeid(base_type),
                               mge::type_name<base_type>(),
                               has_const<T>::value,
                               has_volatile<T>::value,
                               has_reference<T>::value);
    }
} // namespace mge::reflection

template <>
struct fmt::formatter<mge::reflection::type_identifier>
    : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::reflection::type_identifier& id,
                FormatContext&                          ctx) const
    {
        fmt::format_to(ctx.out(),
                       "{}{}{}{}",
                       id.is_const() ? "const " : "",
                       id.is_volatile() ? "volatile " : "",
                       id.name(),
                       id.is_reference() ? "&" : "");
        return ctx.out();
    }
};

namespace std {
    template <> struct hash<mge::reflection::type_identifier>
    {
        std::size_t
        operator()(const mge::reflection::type_identifier& value) const noexcept
        {
            std::size_t hash = 0;
            hash ^= std::hash<std::type_index>()(value.type_index());
            hash ^= value.is_const() ? 7 : 13;
            hash ^= value.is_volatile() ? 17 : 19;
            hash ^= value.is_reference() ? 23 : 29;
            return hash;
        }
    };
} // namespace std
