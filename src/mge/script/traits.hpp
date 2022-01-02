// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include <numeric>
#include <type_traits>

namespace mge::script {

    struct traits
    {
        unsigned int is_void : 1;
        unsigned int is_signed : 1;
        unsigned int is_int8 : 1;
        unsigned int is_int16 : 1;
        unsigned int is_int32 : 1;
        unsigned int is_int64 : 1;
        unsigned int is_float : 1;
        unsigned int is_double : 1;
        unsigned int is_long_double : 1;
        unsigned int is_bool : 1;
        unsigned int is_enum : 1;
    };

    template <typename T> inline constexpr traits traits_of()
    {
        if constexpr (std::is_same_v<T, void>) {
            traits result{.is_void = 1,
                          .is_signed = 0,
                          .is_int8 = 0,
                          .is_int16 = 0,
                          .is_int32 = 0,
                          .is_int64 = 0,
                          .is_float = 0,
                          .is_double = 0,
                          .is_long_double = 0,
                          .is_bool = 0,
                          .is_enum = 0};
            return result;
        } else {
            traits result{
                .is_void = std::is_same_v<T, void> ? 1 : 0,
                .is_signed = std::is_signed_v<T> ? 1 : 0,
                .is_int8 = std::is_same_v<T, char> ||
                                   std::is_same_v<T, unsigned char> ||
                                   std::is_same_v<T, signed char>
                               ? 1
                               : 0,
                .is_int16 = std::numeric_limits<T>::is_integer && sizeof(T) == 2
                                ? 1
                                : 0,
                .is_int32 = std::numeric_limits<T>::is_integer && sizeof(T) == 4
                                ? 1
                                : 0,
                .is_int64 = std::numeric_limits<T>::is_integer && sizeof(T) == 8
                                ? 1
                                : 0,
                .is_float = std::is_same_v<T, float> ? 1 : 0,
                .is_double = std::is_same_v<T, double> ? 1 : 0,
                .is_long_double = std::is_same_v<T, long double> ? 1 : 0,
                .is_bool = std::is_same_v<T, bool> ? 1 : 0,
                .is_enum = std::is_enum_v<T> ? 1 : 0};
            return result;
        }
    }

    template <typename T>
    constexpr bool is_simple_type =
        std::is_same_v<T, void> || std::is_same_v<T, char> ||
        std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char> ||
        std::is_same_v<T, short> || std::is_same_v<T, unsigned short> ||
        std::is_same_v<T, int> || std::is_same_v<T, unsigned int> ||
        std::is_same_v<T, long> || std::is_same_v<T, unsigned long> ||
        std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>;

} // namespace mge::script