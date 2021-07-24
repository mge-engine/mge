// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.

// find_package(glm CONFIG REQUIRED)
// target_link_libraries(main PRIVATE glm::glm)

#pragma once
#include "mge/config.hpp"
#define GLM_FORCE_INLINE
#define GLM_FORCE_SILENT_WARNINGS
#define GLM_ENABLE_EXPERIMENTAL
#pragma warning(disable : 4464)
#pragma warning(disable : 5214)
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace mge {
    using glm::clamp;
    using glm::cross;
    using glm::dot;
    using glm::length;
    using glm::normalize;
} // namespace mge
