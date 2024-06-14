// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <string_view>

namespace mge {

    /**
     * Make a string view from a string literal. The string view is not
     * null-terminated.
     *
     * @tparam N size of string literal
     * @param s string literal
     */
    template <size_t N>
    inline constexpr std::string_view make_string_view(const char (&s)[N])
    {
        if constexpr (N == 0) {
            return std::string_view("");
        } else if (s[N - 1] != '\0') {
            return std::string_view(s, s + N);
        } else {
            if constexpr (N == 1) {
                return std::string_view("");
            }
            const char* s_ptr = static_cast<const char*>(&s[0]);
            return std::string_view(s_ptr, s_ptr + N - 1);
        }
    }
} // namespace mge