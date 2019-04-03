// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/glm.hpp"
#include "mge/math/dllexport.hpp"
#include <iosfwd>

namespace mge {

    using glm::vec4;

    typedef vec4 fvec4;

    MGE_MATH_EXPORT std::ostream& operator <<(std::ostream& os, const fvec4& v);

    inline float squared_length(const fvec4& v)
    {
        return v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w;
    }


 }

