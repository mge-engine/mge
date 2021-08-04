// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/dllexport.hpp"
#include "mge/math/glm.hpp"
#include <iosfwd>

namespace mge {

    using glm::i32vec2;
    using glm::vec2;
    using fvec2 = glm::vec2;
    using ivec2 = glm::i32vec2;

    MGEMATH_EXPORT std::ostream& operator<<(std::ostream& os, const fvec2& v);

    MGEMATH_EXPORT std::ostream& operator<<(std::ostream& os, const ivec2& v);

    inline float squared_length(const fvec2& v) { return v.x * v.x + v.y * v.y; }
} // namespace mge
