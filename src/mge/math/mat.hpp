// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/format.hpp"
#include "mge/math/dllexport.hpp"
#include "mge/math/glm.hpp"

#include <iostream>

namespace mge {

    using fmat2 = glm::mat2;
    using dmat2 = glm::dmat2;
    using fmat3 = glm::mat3;
    using dmat3 = glm::dmat3;
    using fmat4 = glm::mat4;
    using dmat4 = glm::dmat4;

    using fmat2x3 = glm::mat2x3;
    using dmat2x3 = glm::dmat2x3;
    using fmat2x4 = glm::mat2x4;
    using dmat2x4 = glm::dmat2x4;
    using fmat3x2 = glm::mat3x2;
    using dmat3x2 = glm::dmat3x2;
    using fmat3x4 = glm::mat3x4;
    using dmat3x4 = glm::dmat3x4;
    using fmat4x2 = glm::mat4x2;
    using dmat4x2 = glm::dmat4x2;
    using fmat4x3 = glm::mat4x3;
    using dmat4x3 = glm::dmat4x3;

} // namespace mge

template <> struct fmt::formatter<mge::fmat2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fmat2& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}], [{}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[0][1],
                       m[1][1]);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::dmat2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dmat2& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}], [{}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[0][1],
                       m[1][1]);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::fmat3> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fmat3& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}], [{}, {}, {}], [{}, {}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[0][1],
                       m[1][1],
                       m[2][1],
                       m[0][2],
                       m[1][2],
                       m[2][2]);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::dmat3> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dmat3& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}], [{}, {}, {}], [{}, {}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[0][1],
                       m[1][1],
                       m[2][1],
                       m[0][2],
                       m[1][2],
                       m[2][2]);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::fmat4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fmat4& m, FormatContext& ctx) const
    {
        fmt::format_to(
            ctx.out(),
            "[[{}, {}, {}, {}], [{}, {}, {}, {}], [{}, {}, {}, {}], [{}, {}, "
            "{}, {}]]",
            m[0][0],
            m[1][0],
            m[2][0],
            m[3][0],
            m[0][1],
            m[1][1],
            m[2][1],
            m[3][1],
            m[0][2],
            m[1][2],
            m[2][2],
            m[3][2],
            m[0][3],
            m[1][3],
            m[2][3],
            m[3][3]);
        return ctx.out();
    }
};
template <> struct fmt::formatter<mge::dmat4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dmat4& m, FormatContext& ctx) const
    {
        fmt::format_to(
            ctx.out(),
            "[[{}, {}, {}, {}], [{}, {}, {}, {}], [{}, {}, {}, {}], [{}, {}, "
            "{}, {}]]",
            m[0][0],
            m[1][0],
            m[2][0],
            m[3][0],
            m[0][1],
            m[1][1],
            m[2][1],
            m[3][1],
            m[0][2],
            m[1][2],
            m[2][2],
            m[3][2],
            m[0][3],
            m[1][3],
            m[2][3],
            m[3][3]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::fmat2x3> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fmat2x3& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}], [{}, {}], [{}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[0][1],
                       m[1][1],
                       m[0][2],
                       m[1][2]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::dmat2x3> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dmat2x3& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}], [{}, {}], [{}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[0][1],
                       m[1][1],
                       m[0][2],
                       m[1][2]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::fmat2x4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fmat2x4& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}], [{}, {}], [{}, {}], [{}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[0][1],
                       m[1][1],
                       m[0][2],
                       m[1][2],
                       m[0][3],
                       m[1][3]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::dmat2x4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dmat2x4& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}], [{}, {}], [{}, {}], [{}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[0][1],
                       m[1][1],
                       m[0][2],
                       m[1][2],
                       m[0][3],
                       m[1][3]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::fmat3x2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fmat3x2& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}], [{}, {}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[0][1],
                       m[1][1],
                       m[2][1]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::dmat3x2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dmat3x2& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}], [{}, {}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[0][1],
                       m[1][1],
                       m[2][1]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::fmat3x4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fmat3x4& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}], [{}, {}, {}], [{}, {}, {}], [{}, {}, "
                       "{}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[0][1],
                       m[1][1],
                       m[2][1],
                       m[0][2],
                       m[1][2],
                       m[2][2],
                       m[0][3],
                       m[1][3],
                       m[2][3]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::dmat3x4> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dmat3x4& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}], [{}, {}, {}], [{}, {}, {}], [{}, {}, "
                       "{}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[0][1],
                       m[1][1],
                       m[2][1],
                       m[0][2],
                       m[1][2],
                       m[2][2],
                       m[0][3],
                       m[1][3],
                       m[2][3]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::fmat4x2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fmat4x2& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}, {}], [{}, {}, {}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[3][0],
                       m[0][1],
                       m[1][1],
                       m[2][1],
                       m[3][1]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::dmat4x2> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dmat4x2& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}, {}], [{}, {}, {}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[3][0],
                       m[0][1],
                       m[1][1],
                       m[2][1],
                       m[3][1]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::fmat4x3> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::fmat4x3& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}, {}], [{}, {}, {}, {}], [{}, {}, {}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[3][0],
                       m[0][1],
                       m[1][1],
                       m[2][1],
                       m[3][1],
                       m[0][2],
                       m[1][2],
                       m[2][2],
                       m[3][2]);
        return ctx.out();
    }
};
template <>
struct fmt::formatter<mge::dmat4x3> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::dmat4x3& m, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "[[{}, {}, {}, {}], [{}, {}, {}, {}], [{}, {}, {}, {}]]",
                       m[0][0],
                       m[1][0],
                       m[2][0],
                       m[3][0],
                       m[0][1],
                       m[1][1],
                       m[2][1],
                       m[3][1],
                       m[0][2],
                       m[1][2],
                       m[2][2],
                       m[3][2]);
        return ctx.out();
    }
};

namespace mge {

    // 2x2
    inline std::ostream& operator<<(std::ostream& os, const fmat2& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dmat2& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    // 3x3
    inline std::ostream& operator<<(std::ostream& os, const fmat3& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dmat3& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    // 4x4
    inline std::ostream& operator<<(std::ostream& os, const fmat4& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dmat4& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    // 2 cols, 3 rows
    inline std::ostream& operator<<(std::ostream& os, const fmat2x3& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dmat2x3& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    // 2 cols, 4 rows
    inline std::ostream& operator<<(std::ostream& os, const fmat2x4& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dmat2x4& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    // 3 cols, 2 rows
    inline std::ostream& operator<<(std::ostream& os, const fmat3x2& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dmat3x2& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    // 3 cols, 4 rows
    inline std::ostream& operator<<(std::ostream& os, const fmat3x4& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dmat3x4& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    // 4 cols, 2 rows
    inline std::ostream& operator<<(std::ostream& os, const fmat4x2& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dmat4x2& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    // 4 cols, 3 rows
    inline std::ostream& operator<<(std::ostream& os, const fmat4x3& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dmat4x3& m)
    {
        fmt::print(os, "{}", m);
        return os;
    }

} // namespace mge
