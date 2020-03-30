// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/usage.hpp"
#include "opengl.hpp"
#include <tuple>
namespace opengl {
    GLuint         gl_usage(mge::usage p);
    mge::data_type data_type_of_gl_type(GLenum t);
} // namespace opengl
