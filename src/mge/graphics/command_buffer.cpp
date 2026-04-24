// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/command_buffer.hpp"

namespace mge {

    command_buffer::command_buffer(std::pmr::memory_resource* resource)
        : m_pipeline_states(resource)
        , m_programs(resource)
        , m_vertex_buffers(resource)
        , m_index_buffers(resource)
        , m_uniform_blocks(resource)
        , m_textures(resource)
        , m_index_counts(resource)
        , m_index_offsets(resource)
        , m_scissor_rects(resource)
    {}

    void command_buffer::bind_texture(uint32_t      slot,
                                      mge::texture* tex) noexcept
    {
        for (auto& b : m_current_textures) {
            if (b.slot == slot) {
                b.texture = tex;
                return;
            }
        }
        m_current_textures.push_back({slot, tex});
    }

    void command_buffer::draw(const program_handle&       program,
                              const vertex_buffer_handle& vertices,
                              const index_buffer_handle&  indices,
                              uint32_t                    index_count,
                              uint32_t                    index_offset)
    {
        m_programs.push_back(program);
        m_vertex_buffers.push_back(vertices);
        m_index_buffers.push_back(indices);
        m_pipeline_states.push_back(m_current_pipeline_state);
        m_uniform_blocks.push_back(m_current_uniform_block);
        m_textures.push_back(m_current_textures);
        m_index_counts.push_back(index_count);
        m_index_offsets.push_back(index_offset);
        m_scissor_rects.push_back(m_current_scissor_rect);
        m_current_uniform_block = nullptr;
        m_current_textures.clear();
    }

    void command_buffer::depth_write(bool enable) noexcept
    {
        if (enable) {
            m_current_pipeline_state.set(pipeline_state::DEPTH_WRITE);
        } else {
            m_current_pipeline_state.clear(pipeline_state::DEPTH_WRITE);
        }
    }

    void command_buffer::depth_test_function(test func) noexcept
    {
        m_current_pipeline_state.set_depth_test_function(func);
    }

    void command_buffer::cull_face(mge::cull_mode mode) noexcept
    {
        m_current_pipeline_state.set_cull_mode(mode);
    }

} // namespace mge
