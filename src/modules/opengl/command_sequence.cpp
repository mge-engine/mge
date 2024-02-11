#include "command_sequence.hpp"
#include "common.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "opengl.hpp"
#include "program.hpp"
#include "render_context.hpp"
#include "vertex_buffer.hpp"

namespace mge::opengl {

    command_sequence::command_sequence(render_context& context)
        : mge::command_sequence(context)
    {}

    command_sequence::~command_sequence() { delete_all_vaos(); }

    void command_sequence::delete_all_vaos()
    {
        if (!m_all_vaos.empty()) {
            glDeleteVertexArrays(static_cast<GLsizei>(m_all_vaos.size()),
                                 m_all_vaos.data());
            m_all_vaos.clear();
        }
    }

    void command_sequence::clear(const rgba_color& c)
    {
        mge::command_sequence::clear(c);
        m_commands.emplace_back(opengl_command{});
    }

    void command_sequence::draw(const mge::draw_command& command)
    {
        mge::command_sequence::draw(command);
        draw_command c;
        c.program_name = gl_program(command.program());
        c.topology = gl_topology(command.topology());

        c.vao = 0;
        glCreateVertexArrays(1, &c.vao);
        CHECK_OPENGL_ERROR(glCreateVertexArrays);
        m_all_vaos.push_back(c.vao);
        glBindVertexArray(c.vao);
        CHECK_OPENGL_ERROR(glBindVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer(command.vertices()));
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ARRAY_BUFFER));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     gl_index_buffer(command.indices()));
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER));

        const auto& layout = command.vertices()->layout();
        uint32_t    index = 0;
        for (const auto& f : layout.formats()) {
            glEnableVertexAttribArray(index);
            CHECK_OPENGL_ERROR(glEnableVertexAttribArray);
            switch (f.type()) {
            case mge::data_type::FLOAT:
                glVertexAttribPointer(index,
                                      f.size(),
                                      GL_FLOAT,
                                      GL_FALSE,
                                      0,
                                      nullptr);
                CHECK_OPENGL_ERROR(glVertexAttribPointer);
                break;
            default:
                MGE_THROW(opengl::error)
                    << "Unsupported vertex array element type " << f.type();
            }
        }
        glBindVertexArray(0);
        CHECK_OPENGL_ERROR(glBindVertexArray(0));

        c.element_count = (GLsizei)command.indices()->element_count();

        m_commands.emplace_back(c);
    }

} // namespace mge::opengl