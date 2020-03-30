// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/dllexport.hpp"
#include "mge/math/glm.hpp"
#include <iosfwd>

namespace mge {
    using glm::fquat;

    MGE_MATH_EXPORT std::ostream &operator<<(std::ostream &os, const fquat &w);

} // namespace mge
