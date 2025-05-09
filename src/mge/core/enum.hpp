// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/format.hpp"
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

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
    using magic_enum::underlying_type;
    using magic_enum::underlying_type_t;

    using magic_enum::is_scoped_enum;
    using magic_enum::is_scoped_enum_v;
    using magic_enum::is_unscoped_enum;
    using magic_enum::is_unscoped_enum_v;
    using magic_enum::customize::enum_range;

    using magic_enum::enum_flags_cast;
    using magic_enum::enum_flags_contains;
    using magic_enum::enum_flags_name;

    namespace bitwise_enum_operators = magic_enum::bitwise_operators;
} // namespace mge

template <typename E>
    requires std::is_enum_v<E>
struct fmt::formatter<E> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const E& e, FormatContext& ctx) const
    {
        auto name = magic_enum::enum_name(e);
        if (name.empty()) {
            return fmt::format_to(ctx.out(),
                                  "({})0x{:x}",
                                  mge::enum_type_name<E>(),
                                  static_cast<std::underlying_type_t<E>>(e));
        } else {
            return fmt::format_to(ctx.out(), "{}", name);
        }
    }
};

namespace mge {
    template <typename E>
        requires std::is_enum_v<E>
    inline std::ostream& operator<<(std::ostream& os, const E& e)
    {
        fmt::print(os, "{}", e);
        return os;
    }
} // namespace mge
