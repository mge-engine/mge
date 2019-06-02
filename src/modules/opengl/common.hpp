#pragma once
#include "opengl.hpp"
#include "mge/graphics/buffer_change_policy.hpp"
#include "mge/graphics/buffer_access.hpp"
namespace opengl {
    GLuint gl_buffer_change_policy(mge::buffer_change_policy p);
    GLuint gl_buffer_access(mge::buffer_access a);
}
