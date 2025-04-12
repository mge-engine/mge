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

    using fvec2 = glm::vec2;
    using ivec2 = glm::i32vec2;
    using uvec2 = glm::u32vec2;
    using dvec2 = glm::dvec2;

    template <typename T>
    concept vec2_type = std::same_as<T, fvec2> || std::same_as<T, ivec2> ||
                        std::same_as<T, uvec2> || std::same_as<T, dvec2>;

    template <vec2_type T> inline auto squared_length(const T& v)
    {
        return v.x * v.x + v.y * v.y;
    }

    inline std::ostream& operator<<(std::ostream& os, const fvec2& v)
    {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const ivec2& v)
    {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const uvec2& v)
    {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }

} // namespace mge
