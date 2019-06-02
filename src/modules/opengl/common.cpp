// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "common.hpp"
#include "mge/core/log.hpp"

MGE_DEFINE_LOG(OPENGL);

namespace opengl {

    GLuint gl_buffer_change_policy(mge::buffer_change_policy p)
    {
        switch(p) {
        case mge::buffer_change_policy::DEFAULT:
            return GL_STATIC_DRAW;
        case mge::buffer_change_policy::IMMUTABLE:
            return GL_STATIC_DRAW;
        case mge::buffer_change_policy::DYNAMIC:
            return GL_DYNAMIC_DRAW;
        case mge::buffer_change_policy::STAGING:
            return GL_STREAM_DRAW;
        default:
            return GL_STATIC_DRAW;
        }

    }
}
