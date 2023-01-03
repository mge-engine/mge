// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include <memory>
#include <type_traits>

namespace mge {

    template <typename T> struct is_shared_ptr : std::false_type
    {};

    template <typename T>
    struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
    {};

    template <typename T>
    inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

} // namespace mge