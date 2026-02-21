// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/graphics/command_buffer.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/frame_buffer_handle.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/pipeline_state.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/viewport.hpp"

namespace mge {

    /**
     * @brief A pass is a bucket for draw, resource and compute
     * commands.
     *
     * A pass also contains some state information that is set when
     * it is executed:
     * - the rectangle to render to
     * - the scissor rectangle
     * - clear color, depth and stencil values
     * - whether to clear color, depth and stencil buffers
     *
     * A pass is created by a render context and can be used to
     * organize rendering into multiple passes, each with their own
     * clear settings and scissor/viewport rectangles.
     */
    class MGEGRAPHICS_EXPORT pass
    {
    public:
        pass(const pass&) = delete;
        pass(pass&&) noexcept = default;
        pass& operator=(const pass&) = delete;
        pass& operator=(pass&&) = default;

        /**
         * @brief Default destructor.
         */
        ~pass();

        /**
         * @brief Set the render target rectangle.
         *
         * @param r rectangle defining the render area
         */
        void set_rect(const mge::rectangle& r);

        /**
         * @brief Set the viewport for this pass.
         *
         * @param v viewport to use
         */
        void set_viewport(const mge::viewport& v);

        /**
         * @brief Set the viewport to the full render target size.
         */
        void default_viewport();

        /**
         * @brief Set the scissor rectangle.
         *
         * @param r scissor rectangle
         */
        void set_scissor(const mge::rectangle& r);

        /**
         * @brief Set the scissor rectangle to the full render target size.
         */
        void default_scissor();

        /**
         * @brief Set the clear color and enable color clearing.
         *
         * @param color color to clear the color buffer to
         */
        void clear_color(const rgba_color& color);

        /**
         * @brief Disable color buffer clearing for this pass.
         */
        void disable_clear_color();

        /**
         * @brief Set the clear depth value and enable depth clearing.
         *
         * @param depth depth value to clear to
         */
        void clear_depth(float depth);

        /**
         * @brief Disable depth buffer clearing for this pass.
         */
        void disable_clear_depth();

        /**
         * @brief Set the clear stencil value and enable stencil clearing.
         *
         * @param stencil stencil value to clear to
         */
        void clear_stencil(int32_t stencil);

        /**
         * @brief Disable stencil buffer clearing for this pass.
         */
        void disable_clear_stencil();

        /**
         * @brief Set the frame buffer to render into.
         *
         * @param fb frame buffer handle
         */
        void set_frame_buffer(const frame_buffer_handle& fb) noexcept;

        /**
         * @brief Reset this pass to its initial state.
         */
        void reset();

        /**
         * @brief Mark this pass as active without submitting commands.
         *
         * Used to trigger pass execution (e.g. clear operations)
         * even when no draw commands are submitted.
         */
        void touch();

        /**
         * @brief Whether this pass has been activated.
         *
         * @return true if the pass has pending work
         */
        bool active() const noexcept
        {
            return m_active;
        }

        /**
         * @brief Current viewport of this pass.
         *
         * @return viewport reference
         */
        const mge::viewport& viewport() const noexcept
        {
            return m_viewport;
        }

        /**
         * @brief Current scissor rectangle of this pass.
         *
         * @return scissor rectangle reference
         */
        const mge::rectangle& scissor() const noexcept
        {
            return m_scissor;
        }

        /**
         * @brief Current clear color value.
         *
         * @return clear color
         */
        const mge::rgba_color& clear_color_value() const noexcept
        {
            return m_clear_color;
        }

        /**
         * @brief Current clear depth value.
         *
         * @return clear depth
         */
        float clear_depth_value() const noexcept
        {
            return m_clear_depth;
        }

        /**
         * @brief Current clear stencil value.
         *
         * @return clear stencil value
         */
        int32_t clear_stencil_value() const noexcept
        {
            return m_clear_stencil;
        }

        /**
         * @brief Whether color clearing is enabled.
         *
         * @return true if color buffer will be cleared
         */
        bool clear_color_enabled() const noexcept
        {
            return m_clear_color_enabled;
        }

        /**
         * @brief Whether depth clearing is enabled.
         *
         * @return true if depth buffer will be cleared
         */
        bool clear_depth_enabled() const noexcept
        {
            return m_clear_depth_enabled;
        }

        /**
         * @brief Whether stencil clearing is enabled.
         *
         * @return true if stencil buffer will be cleared
         */
        bool clear_stencil_enabled() const noexcept
        {
            return m_clear_stencil_enabled;
        }

        /**
         * @brief Current frame buffer of this pass.
         *
         * @return frame buffer handle
         */
        frame_buffer_handle frame_buffer() const noexcept
        {
            return m_frame_buffer;
        }

        /**
         * @brief Submit a command buffer for execution in this pass.
         *
         * @param command_buffer command buffer containing draw commands
         */
        void submit(const command_buffer& command_buffer);

        /**
         * @brief Iterate over all submitted draw commands.
         *
         * Calls @c f for each draw command with the program,
         * vertex buffer, index buffer, pipeline state, uniform block,
         * and texture.
         *
         * @tparam F callable type
         * @param f callable invoked for each draw command
         */
        template <typename F> void for_each_draw_command(F&& f) const
        {
            for (const auto& cmd : m_draw_commands) {
                f(cmd.program,
                  cmd.vertices,
                  cmd.indices,
                  cmd.state,
                  cmd.uniform_block,
                  cmd.texture,
                  cmd.index_count,
                  cmd.index_offset,
                  cmd.scissor);
            }
        }

    private:
        friend class render_context;
        pass(mge::render_context* context, uint32_t index) noexcept;

        mge::render_context*     m_context{nullptr};
        mge::frame_buffer_handle m_frame_buffer;
        mge::viewport            m_viewport{};
        mge::rectangle           m_scissor{};
        rgba_color               m_clear_color{0.0f, 0.0f, 0.0f, 1.0f};
        float                    m_clear_depth{1.0f};
        int32_t                  m_clear_stencil{0};
        uint32_t                 m_index{0};
        bool                     m_active{false};
        bool                     m_clear_color_enabled{false};
        bool                     m_clear_depth_enabled{false};
        bool                     m_clear_stencil_enabled{false};

        struct draw_command
        {
            program_handle       program;
            vertex_buffer_handle vertices;
            index_buffer_handle  indices;
            mge::pipeline_state  state;
            mge::uniform_block*  uniform_block{nullptr};
            mge::texture*        texture{nullptr};
            uint32_t             index_count{0};
            uint32_t             index_offset{0};
            mge::rectangle       scissor{};
        };

        std::vector<draw_command> m_draw_commands;
    };

} // namespace mge