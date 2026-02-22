// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/cull_mode.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/index_buffer_handle.hpp"
#include "mge/graphics/pipeline_state.hpp"
#include "mge/graphics/program_handle.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/test.hpp"
#include "mge/graphics/vertex_buffer_handle.hpp"

#include <tuple>
#include <vector>

namespace mge {

    class texture;
    class uniform_block;

    /**
     * @brief A command buffer records rendering commands to be
     * submitted to a render context.
     */
    class MGEGRAPHICS_EXPORT command_buffer
    {
    public:
        command_buffer() = default;
        ~command_buffer() = default;

        command_buffer(const command_buffer&) = delete;
        command_buffer(command_buffer&&) noexcept = default;
        command_buffer& operator=(const command_buffer&) = delete;
        command_buffer& operator=(command_buffer&&) = default;

        /**
         * @brief Enable or disable writing to the depth buffer.
         *
         * @param enable true to enable depth writes, false to disable
         */
        void depth_write(bool enable) noexcept;

        /**
         * @brief Set the depth test comparison function.
         *
         * @param func comparison function used for depth testing
         */
        void depth_test_function(test func) noexcept;

        /**
         * @brief Set face culling mode for subsequent draw commands.
         *
         * @param mode culling mode (NONE, CLOCKWISE, or COUNTER_CLOCKWISE)
         */
        void cull_face(mge::cull_mode mode) noexcept;

        /**
         * @brief Set the blend state to opaque (no blending).
         *
         * This is a convenience method that sets the blend state to a default
         * opaque configuration, which is also the default state of the command
         * buffer.
         */
        void blend_opaque()
        {
            m_current_pipeline_state.set_color_blend_operation(
                blend_operation::NONE);
            m_current_pipeline_state.set_color_blend_factors(
                blend_factor::ONE,
                blend_factor::ZERO);
        }

        /**
         * @brief Set the blend function for subsequent draw commands.
         *
         * This sets the blend factors for source and destination colors. The
         * blend operation can be set separately using @c blend_equation().
         *
         * @param src source blend factor
         * @param dst destination blend factor
         */
        void blend_function(blend_factor src, blend_factor dst)
        {
            m_current_pipeline_state.set_blend_factors(src, dst);
        }

        /**
         * @brief Set the blend operation for subsequent draw commands.
         *
         * This sets the blend operation that determines how source and
         * destination colors are combined. The blend factors can be set
         * separately using @c blend_function().
         *
         * @param op blend operation
         */
        void blend_equation(blend_operation op)
        {
            m_current_pipeline_state.set_blend_operation(op);
        }

        /**
         * @brief Bind a uniform block for subsequent draw commands.
         *
         * The block is attached to the next draw() call. After draw(),
         * the binding is cleared.
         *
         * @param block pointer to the uniform block to bind (nullptr to clear)
         */
        void bind_uniform_block(uniform_block* block) noexcept
        {
            m_current_uniform_block = block;
        }

        /**
         * @brief Bind a texture for subsequent draw commands.
         *
         * The texture is attached to the next draw() call. After draw(),
         * the binding is cleared.
         *
         * @param tex pointer to the texture to bind (nullptr to clear)
         */
        void bind_texture(mge::texture* tex) noexcept
        {
            m_current_texture = tex;
        }

        /**
         * @brief Set the scissor rectangle for subsequent draw commands.
         *
         * The scissor rectangle is sticky and persists across draw calls
         * until changed or cleared. A zero-area rectangle (the default)
         * means the pass-level scissor is used.
         *
         * @param r scissor rectangle
         */
        void set_scissor(const mge::rectangle& r) noexcept
        {
            m_current_scissor_rect = r;
        }

        /**
         * @brief Clear the per-draw scissor rectangle.
         *
         * Resets to the zero-area sentinel, meaning subsequent draws
         * will use the pass-level scissor.
         */
        void clear_scissor() noexcept
        {
            m_current_scissor_rect = mge::rectangle{};
        }

        /**
         * @brief Record a draw command into the command buffer.
         *
         * @param program   program to use for drawing
         * @param vertices  vertex buffer to use
         * @param indices   index buffer to use
         * @param index_count number of indices to draw (0 = all)
         * @param index_offset offset in index buffer (in indices, not bytes)
         */
        void draw(const program_handle&       program,
                  const vertex_buffer_handle& vertices,
                  const index_buffer_handle&  indices,
                  uint32_t                    index_count = 0,
                  uint32_t                    index_offset = 0);

        /**
         * @brief Iterate over all recorded draw commands.
         *
         * Calls @c f for each recorded draw command with the program,
         * vertex buffer, index buffer, pipeline state, uniform block,
         * and texture.
         *
         * @tparam F callable type
         * @param f callable invoked for each draw command
         */
        template <typename F> void for_each(F&& f) const
        {
            auto count = m_programs.size();
            for (size_t i = 0; i < count; ++i) {
                f(m_programs[i],
                  m_vertex_buffers[i],
                  m_index_buffers[i],
                  m_pipeline_states[i],
                  m_uniform_blocks[i],
                  m_textures[i],
                  m_index_counts[i],
                  m_index_offsets[i],
                  m_scissor_rects[i]);
            }
        }

        /**
         * @brief Check whether the command buffer has no recorded commands.
         *
         * @return true if no draw commands have been recorded
         */
        bool empty() const noexcept
        {
            return m_programs.empty();
        }

        /**
         * @brief Clear all recorded draw commands.
         */
        void clear() noexcept
        {
            m_programs.clear();
            m_vertex_buffers.clear();
            m_index_buffers.clear();
            m_pipeline_states.clear();
            m_uniform_blocks.clear();
            m_textures.clear();
            m_index_counts.clear();
            m_index_offsets.clear();
            m_scissor_rects.clear();
        }

    private:
        pipeline_state m_current_pipeline_state{pipeline_state::DEFAULT};
        uniform_block* m_current_uniform_block{nullptr};
        mge::texture*  m_current_texture{nullptr};
        mge::rectangle m_current_scissor_rect{};

        std::vector<pipeline_state>       m_pipeline_states;
        std::vector<program_handle>       m_programs;
        std::vector<vertex_buffer_handle> m_vertex_buffers;
        std::vector<index_buffer_handle>  m_index_buffers;
        std::vector<uniform_block*>       m_uniform_blocks;
        std::vector<mge::texture*>        m_textures;
        std::vector<uint32_t>             m_index_counts;
        std::vector<uint32_t>             m_index_offsets;
        std::vector<mge::rectangle>       m_scissor_rects;
    };
} // namespace mge