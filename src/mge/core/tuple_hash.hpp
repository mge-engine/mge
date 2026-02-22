// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <functional>
#include <tuple>

namespace std {
    template <typename... T> struct hash<std::tuple<T...>>
    {
        size_t operator()(const std::tuple<T...>& t) const
        {
            return std::apply(
                [](const auto&... args) {
                    size_t seed = 0;
                    (void)std::initializer_list<int>{
                        (seed ^=
                         std::hash<std::decay_t<decltype(args)>>{}(args) +
                         0x9e3779b9 + (seed << 6) + (seed >> 2),
                         0)...};
                    return seed;
                },
                t);
        }
    };
} // namespace std