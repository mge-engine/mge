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

    using fvec4 = glm::vec4;
    using ivec4 = glm::i32vec4;
    using uvec4 = glm::u32vec4;
    using dvec4 = glm::dvec4;

    template <typename T>
    concept vec4_type = std::same_as<T, fvec4> || std::same_as<T, ivec4> ||
                        std::same_as<T, uvec4> || std::same_as<T, dvec4>;

    template <vec4_type T> inline auto squared_length(const T& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
    }

} // namespace mge

template <> struct fmt::formatter<mge::fvec4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fvec4& v, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {}, {}, {})", v.x, v.y, v.z, v.w);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::ivec4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::ivec4& v, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {}, {}, {})", v.x, v.y, v.z, v.w);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::uvec4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::uvec4& v, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {}, {}, {})", v.x, v.y, v.z, v.w);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::dvec4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dvec4& v, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "({}, {}, {}, {})", v.x, v.y, v.z, v.w);
        return ctx.out();
    }
};

namespace mge {

    inline std::ostream& operator<<(std::ostream& os, const fvec4& v)
    {
        fmt::print(os, "{}", v);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const ivec4& v)
    {
        fmt::print(os, "{}", v);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const uvec4& v)
    {
        fmt::print(os, "{}", v);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dvec4& v)
    {
        fmt::print(os, "{}", v);
        return os;
    }

} // namespace mge
