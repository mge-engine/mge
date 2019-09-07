// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "common.hpp"
#include "mge/core/log.hpp"

MGE_DEFINE_LOG(OPENGL);

namespace opengl {

    GLuint gl_usage(mge::usage p)
    {
        switch(p) {
        case mge::usage::DEFAULT:
            return GL_STATIC_DRAW;
        case mge::usage::IMMUTABLE:
            return GL_STATIC_DRAW;
        case mge::usage::DYNAMIC:
            return GL_DYNAMIC_DRAW;
        case mge::usage::STAGING:
            return GL_STREAM_DRAW;
        default:
            return GL_STATIC_DRAW;
        }
    }

    GLuint gl_buffer_access(mge::buffer_access a)
    {
        switch(a) {
        case mge::buffer_access::READ:
            return GL_READ_ONLY;
        case mge::buffer_access::WRITE:
            return GL_WRITE_ONLY;
        case mge::buffer_access::READ_WRITE:
            return GL_READ_WRITE;
        default:
            return 0; // to raise invalid error
        }
    }
}
