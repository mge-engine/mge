#pragma once
#include "opengl.hpp"
#include "mge/graphics/usage.hpp"
#include "mge/graphics/buffer_access.hpp"
namespace opengl {
    GLuint gl_usage(mge::usage p);
    GLuint gl_buffer_access(mge::buffer_access a);
}
