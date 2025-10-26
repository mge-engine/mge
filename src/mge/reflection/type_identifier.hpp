// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/reflection/dllexport.hpp"

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
                        bool                  is_const,
                        bool                  is_volatile) noexcept;
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

        bool operator==(const type_identifier& other) const noexcept
        {
            return m_type_index == other.m_type_index &&
                   m_is_const == other.m_is_const &&
                   m_is_volatile == other.m_is_volatile;
        }

        bool operator!=(const type_identifier& other) const noexcept
        {
            return !(*this == other);
        }

        bool operator<(const type_identifier& other) const noexcept
        {
            return std::tie(m_type_index, m_is_const, m_is_volatile) <
                   std::tie(other.m_type_index,
                            other.m_is_const,
                            other.m_is_volatile);
        }

        bool operator>(const type_identifier& other) const noexcept
        {
            return std::tie(m_type_index, m_is_const, m_is_volatile) >
                   std::tie(other.m_type_index,
                            other.m_is_const,
                            other.m_is_volatile);
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
        std::type_index m_type_index;
        bool            m_is_const;
        bool            m_is_volatile;
    };

    template <typename T>
    constexpr type_identifier make_type_identifier() noexcept
    {
        return type_identifier(typeid(T),
                               std::is_const_v<T>,
                               std::is_volatile_v<T>);
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
                       "{}{}{}",
                       id.is_const() ? "const " : "",
                       id.is_volatile() ? "volatile " : "",
                       id.type_index().name());
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
            return hash;
        }
    };
} // namespace std
