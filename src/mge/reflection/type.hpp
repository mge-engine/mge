// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

namespace mge::reflection {

    template <typename T> class type
    {
    public:
        type() = default;
        ~type() = default;

        constexpr bool is_void() const noexcept
        {
            return std::is_void_v<T>;
        }

        constexpr bool is_integral() const noexcept
        {
            return std::is_integral_v<T>;
        }
    };

} // namespace mge::reflection