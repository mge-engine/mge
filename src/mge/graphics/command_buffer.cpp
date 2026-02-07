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
    }

} // namespace mge