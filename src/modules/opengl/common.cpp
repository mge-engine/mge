// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "common.hpp"
#include "mge/core/log.hpp"

MGE_DEFINE_LOG(OPENGL);

namespace opengl {

    GLuint gl_buffer_change_policy(mge::buffer_change_policy p)
    {
        return GL_DYNAMIC_DRAW;
    }
}
