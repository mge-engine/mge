// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/command_buffer.hpp"

namespace mge {

    void command_buffer::draw(const program_handle&       program,
                              const vertex_buffer_handle& vertices,
                              const index_buffer_handle&  indices)
    {
        m_programs.push_back(program);
        m_vertex_buffers.push_back(vertices);
        m_index_buffers.push_back(indices);
        m_blend_states.push_back(m_current_blend_state);
        m_draw_flags.push_back(m_current_draw_flags);
    }

    void command_buffer::depth_write(bool enable) noexcept
    {
        if (enable) {
            m_current_draw_flags.set(draw_flags::DEPTH_WRITE);
        } else {
            m_current_draw_flags.clear(draw_flags::DEPTH_WRITE);
        }
    }

} // namespace mge