// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/frame_buffer.hpp"
#include "opengl.hpp"

namespace mge::opengl {

    class render_context_base;

    class frame_buffer : public mge::frame_buffer
    {
    public:
        explicit frame_buffer(render_context_base& ctx);
        ~frame_buffer() override;

        GLuint fbo_name() const noexcept
        {
            return m_fbo;
        }

    protected:
        void on_attach_color(const mge::texture_ref& tex, uint32_t slot) override;
        void on_attach_depth(const mge::texture_ref& tex) override;

    private:
        GLuint m_fbo{0};
    };

} // namespace mge::opengl
