// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/is_shared_ptr.hpp"
#include "mge/core/type_name.hpp"

#include <format>
#include <iostream>
#include <memory>
#include <type_traits>

namespace mge {
    template <typename T> struct gist_type
    {
        const T* value;
    };

    template <typename T> inline gist_type<T> gist(const T& t)
    {
        return gist_type<T>(&t);
    }

    template <typename T>
    concept has_gist_method = requires(T obj, std::format_context& ctx) {
        {
            obj.gist(ctx)
        } -> std::convertible_to<void>;
    };

    template <typename T>
    concept exists_gist_function = requires(T obj, std::format_context& ctx) {
        {
            ::mge::gist(obj, ctx)
        } -> std::convertible_to<void>;
    };
} // namespace mge

template <typename T, typename C>
    requires std::is_arithmetic_v<T>
struct std::formatter<mge::gist_type<T>, C> : public std::formatter<T, C>
{
    template <typename FormatContext>
    auto format(mge::gist_type<T> g, FormatContext& ctx) const
    {
        if (!g.value) {
            return std::format_to(ctx.out(), "nullptr");
        }
        return std::formatter<T, C>::format(*g.value, ctx);
    }
};

template <typename T, typename C>
    requires std::is_pointer_v<T>
struct std::formatter<mge::gist_type<T>, C>
    : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(mge::gist_type<T> g, FormatContext& ctx) const
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
                                  mge::gist(**g.value));
        }
    }
};

template <typename T, typename C>
    requires mge::is_shared_ptr_v<T>
struct std::formatter<mge::gist_type<T>, C>
    : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(mge::gist_type<T> g, FormatContext& ctx) const
    {
        if (!g.value) {
            return std::format_to(ctx.out(), "nullptr");
        } else if (g.value->get() == nullptr) {
            return std::format_to(ctx.out(),
                                  "{} -> nullptr",
                                  mge::type_name<typename T::element_type>());
        } else {
            return std::format_to(ctx.out(),
                                  "std::shared_ptr<{}> -> {}",
                                  mge::type_name<typename T::element_type>(),
                                  mge::gist(**g.value));
        }
    }
};

template <typename T, typename C>
    requires mge::has_gist_method<T> && !mge::exists_gist_function<T>
                                    struct std::formatter<mge::gist_type<T>, C>
    : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(mge::gist_type<T> g, FormatContext& ctx) const
    {
        if (!g.value) {
            return std::format_to(ctx.out(), "nullptr");
        }
        g.value->gist(ctx);
        return ctx.out();
    }
};

template <typename T, typename C>
    requires mge::exists_gist_function<T> &&
             !mge::has_gist_method<T>
             struct std::formatter<mge::gist_type<T>, C>
    : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(mge::gist_type<T> g, FormatContext& ctx) const
    {
        if (!g.value) {
            return std::format_to(ctx.out(), "nullptr");
        }
        ::mge::gist(*g.value, ctx);
        return ctx.out();
    }
};

template <typename T, typename C>
    requires !mge::has_gist_method<T> && !mge::exists_gist_function<T> &&
             !std::is_pointer_v<T> && !mge::is_shared_ptr_v<T> &&
             !std::is_arithmetic_v<T>
             struct std::formatter<mge::gist_type<T>, C>
    : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(mge::gist_type<T> g, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(),
                              "{}@{}",
                              mge::type_name<T>(),
                              (void*)g.value);
    }
};

namespace mge {

    template <typename T>
    inline std::ostream& operator<<(std::ostream&            os,
                                    const mge::gist_type<T>& g)
    {
        std::ostream_iterator<char> out(os);
        std::format_to(out, "{}", g);
        return os;
    }
} // namespace mge
