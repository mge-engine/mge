// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/blend_factor.hpp"
#include "mge/graphics/blend_operation.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/index_buffer_handle.hpp"
#include "mge/graphics/pipeline_state.hpp"
#include "mge/graphics/program_handle.hpp"
#include "mge/graphics/vertex_buffer_handle.hpp"


#include <tuple>

namespace mge {

    /**
     * @brief A command buffer records rendering commands to be
     * submitted to a render context.
     */
    class MGEGRAPHICS_EXPORT command_buffer
    {
    public:
        using blend_state =
            std::tuple<blend_operation, blend_factor, blend_factor>;

        command_buffer() = default;
        ~command_buffer() = default;

        command_buffer(const command_buffer&) = delete;
        command_buffer(command_buffer&&) noexcept = default;
        command_buffer& operator=(const command_buffer&) = delete;
        command_buffer& operator=(command_buffer&&) = default;

        void depth_write(bool enable) noexcept;

        /**
         * @brief Set the blend state to opaque (no blending).
         *
         * This is a convenience method that sets the blend state to a default
         * opaque configuration, which is also the default state of the command
         * buffer.
         */
        void blend_opaque()
        {
            m_current_blend_state = std::make_tuple(blend_operation::NONE,
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
            m_current_blend_state =
                std::make_tuple(std::get<0>(m_current_blend_state), src, dst);
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
            m_current_blend_state =
                std::make_tuple(op,
                                std::get<1>(m_current_blend_state),
                                std::get<2>(m_current_blend_state));
        }

        /**
         * @brief Record a draw command into the command buffer.
         *
         * @param program   program to use for drawing
         * @param vertices  vertex buffer to use
         * @param indices   index buffer to use
         */
        void draw(const program_handle&       program,
                  const vertex_buffer_handle& vertices,
                  const index_buffer_handle&  indices);

        template <typename F> void for_each(F&& f) const
        {
            auto count = m_programs.size();
            for (size_t i = 0; i < count; ++i) {
                f(m_programs[i],
                  m_vertex_buffers[i],
                  m_index_buffers[i],
                  m_blend_states[i],
                  m_pipeline_states[i]);
            }
        }

        bool empty() const noexcept
        {
            return m_programs.empty();
        }

        void clear() noexcept
        {
            m_programs.clear();
            m_vertex_buffers.clear();
            m_index_buffers.clear();
            m_blend_states.clear();
            m_pipeline_states.clear();
        }

    private:
        blend_state m_current_blend_state{
            blend_operation::NONE, blend_factor::ONE, blend_factor::ZERO};
        pipeline_state m_current_pipeline_state{pipeline_state::DEFAULT};

        std::vector<pipeline_state>       m_pipeline_states;
        std::vector<blend_state>          m_blend_states;
        std::vector<program_handle>       m_programs;
        std::vector<vertex_buffer_handle> m_vertex_buffers;
        std::vector<index_buffer_handle>  m_index_buffers;
    };
} // namespace mge