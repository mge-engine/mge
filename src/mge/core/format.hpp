// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <format>
#include <iostream>
#include <string_view>
#include <type_traits>
namespace mge {

    template <typename T>
    concept has_format_method =
        requires(const T& obj, std::format_context& ctx) {
            { obj.format(ctx) } -> std::convertible_to<void>;
        };

} // namespace mge
namespace std {
    template <typename T>
        requires mge::has_format_method<T>
    struct formatter<T>
    {
        constexpr auto parse(std::format_parse_context& ctx)
        {
            return ctx.end();
        }

        template <typename FormatContext>
        auto format(const T& t, FormatContext& ctx) const
        {
            t.format(ctx);
            return ctx.out();
        }
    };
} // namespace std

namespace mge {

    template <typename T>
        requires mge::has_format_method<T>
    inline std::ostream& operator<<(std::ostream& os, const T& t)
    {
        std::ostream_iterator<char> out(os);
        std::format_to(out, "{}", t);
        return os;
    }
} // namespace mge
