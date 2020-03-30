// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "common.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stdexceptions.hpp"

MGE_DEFINE_LOG(OPENGL);

namespace opengl {

    GLuint gl_usage(mge::usage p)
    {
        switch (p) {
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

    mge::data_type data_type_of_gl_type(GLenum t)
    {
        switch (t) {
        case GL_FLOAT_VEC2:
            return mge::data_type::FLOAT_VEC2;
        case GL_FLOAT_VEC3:
            return mge::data_type::FLOAT_VEC3;
        case GL_FLOAT_VEC4:
            return mge::data_type::FLOAT_VEC4;
        default:
            MGE_THROW(mge::illegal_argument)
                << std::hex << t << " is not a valid data type";
        }
    }
} // namespace opengl
