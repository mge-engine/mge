// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/glm.hpp"
#include "mge/math/dllexport.hpp"
#include <iosfwd>
namespace mge {

    using glm::vec2;
    using glm::i32vec2;
    using fvec2 = glm::vec2;
    using ivec2 = glm::i32vec2;
    
    MGE_MATH_EXPORT std::ostream& operator <<(std::ostream& os, const fvec2& v);
    MGE_MATH_EXPORT std::ostream& operator <<(std::ostream& os, const ivec2& v);

    inline float squared_length(const fvec2& v)
    {
        return v.x*v.x + v.y*v.y;
    }
 }

