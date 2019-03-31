#pragma once
#include "mge/config.hpp"
#define GLM_FORCE_INLINE
#define GLM_FORCE_SILENT_WARNINGS
#pragma warning( disable : 4464)
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace mge {
    using glm::clamp;
}