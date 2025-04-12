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

    using fvec3 = glm::vec3;
    using ivec3 = glm::i32vec3;
    using uvec3 = glm::u32vec3;
    using dvec3 = glm::dvec3;

    template <typename T>
    concept vec3_type = std::same_as<T, fvec3> || std::same_as<T, ivec3> ||
                        std::same_as<T, uvec3> || std::same_as<T, dvec3>;

    template <vec3_type T> inline auto squared_length(const T& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

    inline std::ostream& operator<<(std::ostream& os, const fvec3& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const ivec3& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const uvec3& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
    inline std::ostream& operator<<(std::ostream& os, const dvec3& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

} // namespace mge
