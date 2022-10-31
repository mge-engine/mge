#pragma once
#include "mge/graphics/topology.hpp"
#include "opengl.hpp"
namespace mge::opengl {

    inline constexpr GLuint gl_topology(mge::topology t)
    {
        switch (t) {
        case mge::topology::TRIANGLES:
            return GL_TRIANGLES;
        case mge::topology::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case mge::topology::LINES:
            return GL_LINES;
        case mge::topology::LINE_STRIP:
            return GL_LINE_STRIP;
        default:
            return 0;
        }
    }

} // namespace mge::opengl