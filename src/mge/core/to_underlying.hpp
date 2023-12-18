// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <type_traits>
#include <utility>
namespace mge {
#if __cpp_lib_to_underlying >= 202102L
    using std::to_underlying;
#else
    template <typename E>
    constexpr auto to_underlying(E e) -> typename std::underlying_type<E>::type
    {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }
#endif
} // namespace mge
