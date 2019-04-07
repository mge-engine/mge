// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#define GLM_FORCE_INLINE
#define GLM_FORCE_SILENT_WARNINGS
#pragma warning( disable : 4464)
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mge {
    using glm::clamp;
    using glm::length;
    using glm::dot;
    using glm::cross;
    using glm::normalize;
}
