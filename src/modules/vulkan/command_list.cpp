// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "error.hpp"
#include "frame_command_list.hpp"
#include "index_buffer.hpp"
#include "program.hpp"
#include "render_context.hpp"
#include "shader.hpp"
#include "vertex_buffer.hpp"

namespace mge::vulkan {
    command_list::command_list(render_context& context)
        : mge::command_list(context, false)
        , m_vulkan_context(context)
        , m_color_set(false)
    {}

    command_list::~command_list() {}

    void command_list::clear(const rgba_color& c)
    {
        m_color_set = true;
        m_clear_color = c;
    }

    void command_list::draw(const mge::draw_command& command)
    {
        m_draw_commands.push_back(command);
    }

    void command_list::execute()
    {
        if (m_draw_commands.empty() && !m_color_set) {
            return;
        }
        auto frame_cmds = m_vulkan_context.create_current_frame_command_list();
        if (m_color_set) {
            frame_cmds->clear(m_clear_color);
        }
        for (const auto& command : m_draw_commands) {
            frame_cmds->draw(command);
        }
        frame_cmds->finish();
        frame_cmds->execute();
    }

    void command_list::finish() { return; }

    void command_list::scissor(const mge::rectangle& rect) { m_scissor = rect; }

    void command_list::viewport(const mge::viewport& vp) { m_viewport = vp; }

    void command_list::default_scissor()
    {
        auto ext = m_vulkan_context.extent();
        m_scissor.left = 0;
        m_scissor.top = 0;
        m_scissor.right = ext.width;
        m_scissor.bottom = ext.height;
    }

} // namespace mge::vulkan