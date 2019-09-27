#pragma once
#include "opengl.hpp"
#include "mge/graphics/usage.hpp"
#include "mge/graphics/data_type.hpp"
namespace opengl {
    GLuint gl_usage(mge::usage p);
    mge::data_type data_type_of_gl_type(GLenum t);
}
