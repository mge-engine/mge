#include "command_sequence.hpp"
#include "render_context.hpp"

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
    }

} // namespace mge::opengl