// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace mge {

    template <typename T> struct is_primitive_type : std::false_type
    {};

    template <> struct is_primitive_type<bool> : std::true_type
    {};
    template <> struct is_primitive_type<int8_t> : std::true_type
    {};
    template <> struct is_primitive_type<uint8_t> : std::true_type
    {};
    template <> struct is_primitive_type<int16_t> : std::true_type
    {};
    template <> struct is_primitive_type<uint16_t> : std::true_type
    {};
    template <> struct is_primitive_type<int32_t> : std::true_type
    {};
    template <> struct is_primitive_type<uint32_t> : std::true_type
    {};
    template <> struct is_primitive_type<int64_t> : std::true_type
    {};
    template <> struct is_primitive_type<uint64_t> : std::true_type
    {};
    template <> struct is_primitive_type<float> : std::true_type
    {};
    template <> struct is_primitive_type<double> : std::true_type
    {};
    template <> struct is_primitive_type<long double> : std::true_type
    {};
    template <> struct is_primitive_type<std::string> : std::true_type
    {};
    template <> struct is_primitive_type<std::string_view> : std::true_type
    {};

    template <typename T>
    inline constexpr bool is_primitive_type_v = is_primitive_type<T>::value;

    template <typename T> struct is_primitive_vector : std::false_type
    {};

    template <typename T>
    struct is_primitive_vector<std::vector<T>>
        : std::bool_constant<is_primitive_type_v<T>>
    {};

    template <typename T>
    inline constexpr bool is_primitive_vector_v = is_primitive_vector<T>::value;

} // namespace mge
