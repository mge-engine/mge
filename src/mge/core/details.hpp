// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/is_shared_ptr.hpp"
#include "mge/core/type_name.hpp"
#include <iostream>
#include <memory>
#include <type_traits>
namespace mge {

    template <typename T> struct details_type
    {
        const T* value;
    };

    template <typename T> inline details_type<T> details(const T& t)
    {
        return details_type<T>(&t);
    }

    template <typename T>
    concept has_details_method = requires(T obj, std::format_context& ctx) {
        {
            obj.details(ctx)
        } -> std::convertible_to<void>;
    };

} // namespace mge

template <typename T, typename C>
    requires std::is_arithmetic_v<T>
struct std::formatter<mge::details_type<T>, C> : public std::formatter<T, C>
{
    template <typename FormatContext>
    auto format(mge::details_type<T> g, FormatContext& ctx) const
    {
        if (!g.value) {
            return std::format_to(ctx.out(), "nullptr");
        }
        return std::formatter<T, C>::format(*g.value, ctx);
    }
};

template <typename T, typename C>
    requires std::is_pointer_v<T>
struct std::formatter<mge::details_type<T>, C>
    : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(mge::details_type<T> g, FormatContext& ctx) const
    {
        if (!g.value) {
            return std::format_to(ctx.out(), "nullptr");
        } else if (*g.value == nullptr) {
            return std::format_to(ctx.out(),
                                  "{} -> nullptr",
                                  mge::type_name<T>());
        } else {
            return std::format_to(ctx.out(),
                                  "{} -> {}",
                                  mge::type_name<T>(),
                                  mge::details(**g.value));
        }
    }
};

template <typename T, typename C>
    requires mge::is_shared_ptr_v<T>
struct std::formatter<mge::details_type<T>, C>
    : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(mge::details_type<T> g, FormatContext& ctx) const
    {
        if (!g.value) {
            return std::format_to(ctx.out(), "nullptr");
        } else if (g.value->get() == nullptr) {
            return std::format_to(ctx.out(),
                                  "{} -> nullptr",
                                  mge::type_name<T::element_type>());
        } else {
            return std::format_to(ctx.out(),
                                  "std::shared_ptr<{}> -> {}",
                                  mge::type_name<T::element_type>(),
                                  mge::details(**g.value));
        }
    }
};

template <typename T, typename C>
    requires mge::has_details_method<T>
struct std::formatter<mge::details_type<T>, C>
    : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(mge::details_type<T> g, FormatContext& ctx) const
    {
        if (!g.value) {
            return std::format_to(ctx.out(), "nullptr");
        }
        g.value->details(ctx);
        return ctx.out();
    }
};

template <typename T, typename C>
    requires !mge::has_details_method<T> && !std::is_pointer_v<T> &&
             !mge::is_shared_ptr_v<T> &&
             !std::is_arithmetic_v<T>
             struct std::formatter<mge::details_type<T>, C>
    : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(mge::details_type<T> g, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(),
                              "{}@{}",
                              mge::type_name<T>(),
                              (void*)g.value);
    }
};

namespace mge {

    template <typename T>
    inline std::ostream& operator<<(std::ostream&               os,
                                    const mge::details_type<T>& g)
    {
        std::ostream_iterator<char> out(os);
        g.format(out);
        std::format_to(out, "{}", g);
        return os;
    }
} // namespace mge
