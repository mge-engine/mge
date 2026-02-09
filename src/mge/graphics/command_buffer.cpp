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
        m_pipeline_states.push_back(m_current_pipeline_state);
    }

    void command_buffer::depth_write(bool enable) noexcept
    {
        if (enable) {
            m_current_pipeline_state.set(pipeline_state::DEPTH_WRITE);
        } else {
            m_current_pipeline_state.clear(pipeline_state::DEPTH_WRITE);
        }
    }

} // namespace mge