// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/format.hpp"
#include "mge/math/dllexport.hpp"
#include "mge/math/glm.hpp"

#include <concepts>
#include <iostream>
#include <type_traits>

namespace mge {

    using fvec2 = glm::vec2;
    using ivec2 = glm::i32vec2;
    using uvec2 = glm::u32vec2;
    using dvec2 = glm::dvec2;

    template <typename T>
    concept vec2_type = std::same_as<T, fvec2> || std::same_as<T, ivec2> ||
                        std::same_as<T, uvec2> || std::same_as<T, dvec2>;

    template <vec2_type T> inline auto squared_length(const T& v)
    {
        return v.x * v.x + v.y * v.y;
    }

} // namespace mge

template <> struct fmt::formatter<mge::fvec2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fvec2& v, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {})", v.x, v.y);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::ivec2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::ivec2& v, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {})", v.x, v.y);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::uvec2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::uvec2& v, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {})", v.x, v.y);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::dvec2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dvec2& v, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {})", v.x, v.y);
        return ctx.out();
    }
};

namespace mge {

    inline std::ostream& operator<<(std::ostream& os, const fvec2& v)
    {
        fmt::print(os, "{}", v);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const ivec2& v)
    {
        fmt::print(os, "{}", v);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const uvec2& v)
    {
        fmt::print(os, "{}", v);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dvec2& v)
    {
        fmt::print(os, "{}", v);
        return os;
    }

} // namespace mge
