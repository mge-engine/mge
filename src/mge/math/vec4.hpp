// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/dllexport.hpp"
#include "mge/math/glm.hpp"

#include <concepts>
#include <iostream>
#include <type_traits>
namespace mge {

    using fvec4 = glm::vec4;
    using ivec4 = glm::i32vec4;
    using uvec4 = glm::u32vec4;
    using dvec4 = glm::dvec4;

    concept vec4_type =
        std::same_as<fvec4, fvec4> || std::same_as<ivec4, ivec4> ||
        std::same_as<uvec4, uvec4> || std::same_as<dvec4, dvec4>;

    template <vec4_type T>
    inline std::ostream& operator<<(std::ostream& os, const T& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        return os;
    }

    template <vec4_type T> inline auto squared_length(const T& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
    }
} // namespace mge
