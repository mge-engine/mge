// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"

namespace mge {

    /**
     * @brief Wrapper class for markdown formatting.
     */
    template <typename T> struct markdown
    {
        const T& value;
    };

    template <typename T> markdown(const T&) -> markdown<T>;

} // namespace mge

template <typename T> struct fmt::formatter<mge::markdown<T>>
{
    template <typename ParseContext> constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mge::markdown<T>& m, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", m.value);
    }
};