// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <type_traits>
namespace mge {

    template <typename T> class is_callable
    {
    private:
        template <typename C> static char test(decltype(&C::operator()));
        template <typename C> static int  test(...);

    public:
        static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    template <typename T>
    inline constexpr bool is_callable_v = is_callable<T>::value;

} // namespace mge