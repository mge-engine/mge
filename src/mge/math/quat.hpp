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

    using fquat = glm::quat;
    using dquat = glm::dquat;

    template <typename T>
    concept quat_type = std::same_as<T, fquat> || std::same_as<T, dquat>;

    inline std::ostream& operator<<(std::ostream& os, const fquat& q)
    {
        os << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const dquat& q)
    {
        os << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
        return os;
    }

} // namespace mge
