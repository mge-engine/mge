// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/glm.hpp"
#include "mge/math/vec3.hpp"
namespace mge {
    using glm::identity;
    using glm::ortho;
    using glm::perspective;
    using glm::rotate;
    using glm::scale;
    using glm::translate;

    inline auto look_at(const fvec3 &eye, const fvec3 &center, const fvec3 &up)
    {
        return glm::lookAt(eye, center, up);
    }
} // namespace mge
