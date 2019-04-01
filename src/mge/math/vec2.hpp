// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/glm.hpp"
#include "mge/math/dllexport.hpp"
#include <iosfwd>
namespace mge {

    using glm::vec2;
    typedef vec2 fvec2;

    MGE_MATH_EXPORT std::ostream& operator <<(std::ostream& os, const fvec2& v);
 }

