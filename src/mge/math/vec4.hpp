// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/dllexport.hpp"
#include "mge/math/glm.hpp"
#include <iosfwd>

namespace mge {

    using glm::vec4;

    using fvec4 = vec4;

    MGEMATH_EXPORT std::ostream &operator<<(std::ostream &os, const fvec4 &v);

    inline float squared_length(const fvec4 &v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
    }

} // namespace mge
