// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/dllexport.hpp"
#include "mge/math/glm.hpp"
#include <iosfwd>

namespace mge {
    using fmat4 = glm::fmat4;
    using glm::determinant;
    using glm::inverse;
    using glm::inversesqrt;
    using glm::transpose;

    MGE_MATH_EXPORT std::ostream &operator<<(std::ostream &os, const fmat4 &m);
} // namespace mge
