// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include <type_traits>

namespace mge::script {

    struct traits
    {
        unsigned int is_void : 1;
    };

    template <typename T> inline constexpr traits traits_of()
    {
        traits result{.is_void = std::is_same_v<T, void> ? 1 : 0};
        return result;
    }

    template <typename T>
    constexpr bool is_simple_type =
        std::is_same_v<T, void> || std::is_same_v<T, char>;

} // namespace mge::script