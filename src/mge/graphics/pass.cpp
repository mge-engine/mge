// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/pass.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    pass::pass(mge::render_context* context, uint32_t index) noexcept
        : m_context(context)
        , m_index(index)
    {}

    pass::~pass() {}

    void pass::touch()
    {
        m_active = true;
    }

    void pass::set_rect(const mge::rectangle& r)
    {
        m_viewport.set_rect(r);
    }

    void pass::set_viewport(const mge::viewport& v)
    {
        m_viewport = v;
    }

    void pass::default_viewport()
    {
        m_viewport = m_context->default_viewport();
    }

    void pass::set_scissor(const mge::rectangle& r)
    {
        m_scissor = r;
    }

    void pass::default_scissor()
    {
        m_scissor = m_context->default_scissor();
    }

    void pass::reset()
    {
        m_active = false;
        m_viewport = mge::viewport{0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
        m_scissor = mge::rectangle{0, 0, 1, 1};
        m_clear_color = rgba_color{0.0f, 0.0f, 0.0f, 1.0f};
        m_clear_depth = 1.0f;
        m_clear_stencil = 0;
        m_clear_color_enabled = false;
        m_clear_depth_enabled = false;
        m_clear_stencil_enabled = false;
        m_draw_commands.clear();
    }

    void pass::clear_color(const rgba_color& color)
    {
        m_clear_color = color;
        m_clear_color_enabled = true;
        m_active = true;
    }

    void pass::disable_clear_color()
    {
        m_clear_color_enabled = false;
    }

    void pass::clear_depth(float depth)
    {
        m_clear_depth = depth;
        m_clear_depth_enabled = true;
        m_active = true;
    }

    void pass::disable_clear_depth()
    {
        m_clear_depth_enabled = false;
    }

    void pass::clear_stencil(int32_t stencil)
    {
        m_clear_stencil = stencil;
        m_clear_stencil_enabled = true;
        m_active = true;
    }

    void pass::disable_clear_stencil()
    {
        m_clear_stencil_enabled = false;
    }

    void pass::set_frame_buffer(const frame_buffer_handle& fb) noexcept
    {
        m_frame_buffer = fb;
        // setting frame buffer does not make a pass active
    }

    void pass::submit(const command_buffer& cb)
    {
        if (!cb.empty()) {
            cb.for_each([this](const program_handle&       program,
                               const vertex_buffer_handle& vertices,
                               const index_buffer_handle&  indices,
                               const pipeline_state&       state,
                               mge::uniform_block*         ub) {
                m_draw_commands.push_back(
                    draw_command{program, vertices, indices, state, ub});
            });
        }
        m_active = true;
    }

} // namespace mge