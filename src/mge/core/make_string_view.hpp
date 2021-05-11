// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <string_view>

namespace mge {
    template <size_t N>
    inline std::string_view make_string_view(const char (&s)[N])
    {
        if constexpr (N == 0 || N == 1) {
            return std::string_view();
        } else {
            const char *s_ptr = static_cast<const char *>(&s[0]);
            return std::string_view(s_ptr, s_ptr + N - 1);
        }
    }
} // namespace mge