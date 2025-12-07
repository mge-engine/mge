// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/glfw/glfw_instance.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace mge {
    MGE_DEFINE_TRACE(GLFW);
}

namespace mge::glfw {

    glfw_instance::glfw_instance()
    {
        MGE_DEBUG_TRACE(GLFW, "Initializing GLFW");
        if (!glfwInit()) {
            MGE_THROW(mge::runtime_exception) << "Failed to initialize GLFW";
        }
    }

    glfw_instance::~glfw_instance()
    {
        MGE_DEBUG_TRACE(GLFW, "Terminating GLFW");
        glfwTerminate();
    }

    glfw_instance& glfw_instance::instance()
    {
        static glfw_instance s_instance;
        return s_instance;
    }

} // namespace mge::glfw
