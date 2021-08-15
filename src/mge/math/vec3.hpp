// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/dllexport.hpp"
#include "mge/math/glm.hpp"
#include <iosfwd>

namespace mge {

    using glm::vec3;

    using fvec3 = vec3;

    MGEMATH_EXPORT std::ostream& operator<<(std::ostream& os, const fvec3& v);

    inline auto squared_length(const fvec3& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

} // namespace mge
