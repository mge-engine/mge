#include "mge/graphics/command_sequence.hpp"
namespace mge {

    void command_sequence::clear(const rgba_color& c)
    {
        m_commands.push_back(clear_command{c});
    }

    void command_sequence::draw(const mge::draw_command& command)
    {
        m_commands.push_back(draw_command{command});
    }
}