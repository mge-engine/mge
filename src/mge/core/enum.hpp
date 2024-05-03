// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/format.hpp"
#include <magic_enum.hpp>
#include <magic_enum_flags.hpp>

namespace mge {
    using magic_enum::enum_cast;
    using magic_enum::enum_contains;
    using magic_enum::enum_count;
    using magic_enum::enum_entries;
    using magic_enum::enum_index;
    using magic_enum::enum_integer;
    using magic_enum::enum_name;
    using magic_enum::enum_type_name;
    using magic_enum::enum_values;
    using magic_enum::is_scoped_enum;
    using magic_enum::is_scoped_enum_v;
    using magic_enum::is_unscoped_enum;
    using magic_enum::is_unscoped_enum_v;
    using magic_enum::customize::enum_range;

    using magic_enum::enum_flags_cast;
    using magic_enum::enum_flags_contains;
    using magic_enum::enum_flags_name;

    namespace bitwise_enum_operators = magic_enum::bitwise_operators;

    template <typename E> using provide_formatter = std::true_type;
} // namespace mge

template <typename E>
    requires mge::provide_formatter<E>::value &&
             (mge::is_scoped_enum_v<E> || mge::is_unscoped_enum_v<E>)
struct std::formatter<E> : public std::formatter<std::string_view>
{
    constexpr auto format_if_valid(E e, std::format_context& ctx) const
    {
        if (mge::enum_contains(e)) {
            std::format_to(ctx.out(), "{}", mge::enum_name(e));
            return true;
        } else {
            return false;
        }
    }

    template <typename FormatContext> auto format(E e, FormatContext& ctx) const
    {
        if (!format_if_valid(e, ctx)) {
            using ut = std::underlying_type<E>::type;
            ut tmp = static_cast<ut>(e);
            std::format_to(ctx.out(),
                           "({})0x{:x}",
                           mge::enum_type_name<E>(),
                           tmp);
        }
        return ctx.out();
    }
};

namespace mge {
    template <typename E>
        requires mge::provide_formatter<E>::value &&
                 (mge::is_scoped_enum_v<E> || mge::is_unscoped_enum_v<E>)
    inline std::ostream& operator<<(std::ostream& os, const E& e)
    {
        std::ostream_iterator<char> out(os);
        std::format_to(out, "{}", e);
        return os;
    }
} // namespace mge
