// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/glm.hpp"
#include "mge/math/dllexport.hpp"
#include <iosfwd>

namespace mge {

    using glm::vec3;

    using fvec3 = vec3;

    MGE_MATH_EXPORT std::ostream& operator <<(std::ostream& os, const fvec3& v);

    inline auto squared_length(const fvec3& v)
    {
        return v.x*v.x + v.y*v.y + v.z*v.z;
    }


 }

