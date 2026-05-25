// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "frame_buffer.hpp"
#include "error.hpp"
#include "render_context.hpp"
#include "texture.hpp"

namespace mge::opengl {

    frame_buffer::frame_buffer(render_context& ctx)
        : mge::frame_buffer(ctx)
    {
        glGenFramebuffers(1, &m_fbo);
        CHECK_OPENGL_ERROR(glGenFramebuffers);
    }

    frame_buffer::~frame_buffer()
    {
        if (m_fbo) {
            glDeleteFramebuffers(1, &m_fbo);
        }
    }

    void frame_buffer::on_attach_color(const mge::texture_ref& tex, uint32_t slot)
    {
        auto* gl_tex = static_cast<opengl::texture*>(tex.get());
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        CHECK_OPENGL_ERROR(glBindFramebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + slot,
                               GL_TEXTURE_2D,
                               gl_tex->texture_name(),
                               0);
        CHECK_OPENGL_ERROR(glFramebufferTexture2D);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CHECK_OPENGL_ERROR(glBindFramebuffer);
    }

    void frame_buffer::on_attach_depth(const mge::texture_ref& tex)
    {
        auto*  gl_tex = static_cast<opengl::texture*>(tex.get());
        GLenum attachment = gl_tex->is_depth_stencil()
                                ? GL_DEPTH_STENCIL_ATTACHMENT
                                : GL_DEPTH_ATTACHMENT;
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        CHECK_OPENGL_ERROR(glBindFramebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               attachment,
                               GL_TEXTURE_2D,
                               gl_tex->texture_name(),
                               0);
        CHECK_OPENGL_ERROR(glFramebufferTexture2D);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CHECK_OPENGL_ERROR(glBindFramebuffer);
    }

} // namespace mge::opengl
