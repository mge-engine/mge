// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_command_list.hpp"
#include "mge/core/stdexceptions.hpp"
namespace mge {

    memory_command_list::memory_command_list(render_context &context)
        : command_list(context, false)
    {}

    void memory_command_list::clear(const rgba_color &c)
    {
        clear_data data{c};
        m_elements.push_back(data);
    }

    void memory_command_list::clear_depth(float value)
    {
        clear_depth_data data{value};
        m_elements.push_back(data);
    }

    void memory_command_list::clear_stencil(int32_t value)
    {
        clear_stencil_data data{value};
        m_elements.push_back(data);
    }

    void memory_command_list::draw(const draw_command &c)
    {
        draw_data data(c);
        m_elements.push_back(data);
    }

    void memory_command_list::draw(draw_command &&c)
    {
        draw_data data(c);
        m_elements.push_back(data);
    }

    void memory_command_list::on_finish() {}

    void memory_command_list::on_play(const command_list_ref &commands)
    {
        play_command_list_data data{commands};
        m_elements.push_back(data);
    }

    void memory_command_list::on_clear() { m_elements.clear(); }
} // namespace mge
