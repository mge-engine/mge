// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "opengl.hpp"
#include "mge/graphics/usage.hpp"
#include "mge/graphics/data_type.hpp"
#include <tuple>
namespace opengl {
    GLuint gl_usage(mge::usage p);
    mge::data_type data_type_of_gl_type(GLenum t);
}
