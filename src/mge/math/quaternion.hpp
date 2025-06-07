// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/glm.hpp"

namespace mge {
    using fquat = glm::fquat;
    using dquat = glm::dquat;
    using quat = glm::quat;

    constexpr fquat identity_rotation = fquat(1.0f, 0.0f, 0.0f, 0.0f);
} // namespace mge