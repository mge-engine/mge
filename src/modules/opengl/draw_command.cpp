#include "draw_command.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"

namespace mge::opengl {
    draw_command::draw_command(mge::render_context&     context,
                               const program_ref&       program,
                               const vertex_buffer_ref& vertices,
                               const index_buffer_ref&  indices,
                               mge::topology            t)
        : mge::draw_command(context, program, vertices, indices, t)
        , m_vao(0)
    {
        // glGenVertexArrays(1, &m_vao);
    }

    draw_command::~draw_command()
    { // glDeleteVertexArrays(1, &m_vao);
    }

    void draw_command::prepare() const
    {
        glCreateVertexArrays(1, &m_vao);
        CHECK_OPENGL_ERROR(glCreateVertexArrays);
        glBindVertexArray(m_vao);
        CHECK_OPENGL_ERROR(glBindVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer(m_vertices));
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ARRAY_BUFFER));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer(m_indices));
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER));
    }

} // namespace mge::opengl