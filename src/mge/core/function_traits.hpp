// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/nth_type.hpp"
namespace mge {

    template <typename T>
    struct function_traits : public function_traits<decltype(&T::operator())>
    {};

    template <typename C, typename R, typename... Args>
    struct function_traits<R (C::*)(Args...) const>
    {
        static constexpr auto arity = sizeof...(Args);
        using result_type = R;

        template <size_t I> struct arg
        {
            using type = nth_type<I, Args...>::type;
        };
    };
} // namespace mge