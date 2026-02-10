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

        void set_rect(const mge::rectangle& r);
        void set_viewport(const mge::viewport& v);
        void default_viewport();

        void set_scissor(const mge::rectangle& r);
        void default_scissor();

        void clear_color(const rgba_color& color);
        void disable_clear_color();

        void clear_depth(float depth);
        void disable_clear_depth();

        void clear_stencil(int32_t stencil);
        void disable_clear_stencil();

        void set_frame_buffer(const frame_buffer_handle& fb) noexcept;

        void reset();

        void touch();

        bool active() const noexcept
        {
            return m_active;
        }

        const mge::viewport& viewport() const noexcept
        {
            return m_viewport;
        }

        const mge::rectangle& scissor() const noexcept
        {
            return m_scissor;
        }

        const mge::rgba_color& clear_color_value() const noexcept
        {
            return m_clear_color;
        }

        float clear_depth_value() const noexcept
        {
            return m_clear_depth;
        }

        int32_t clear_stencil_value() const noexcept
        {
            return m_clear_stencil;
        }

        bool clear_color_enabled() const noexcept
        {
            return m_clear_color_enabled;
        }

        bool clear_depth_enabled() const noexcept
        {
            return m_clear_depth_enabled;
        }

        bool clear_stencil_enabled() const noexcept
        {
            return m_clear_stencil_enabled;
        }

        frame_buffer_handle frame_buffer() const noexcept
        {
            return m_frame_buffer;
        }

        void submit(const command_buffer& command_buffer);

        template <typename F> void for_each_draw_command(F&& f) const
        {
            for (const auto& cmd : m_draw_commands) {
                f(cmd.program,
                  cmd.vertices,
                  cmd.indices,
                  cmd.state);
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
            program_handle              program;
            vertex_buffer_handle        vertices;
            index_buffer_handle         indices;
            mge::pipeline_state         state;
        };

        std::vector<draw_command> m_draw_commands;
    };

} // namespace mge