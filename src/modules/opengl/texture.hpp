// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/texture.hpp"
#include "opengl.hpp"

namespace mge::opengl {
    class render_context;

    class texture : public mge::texture
    {
    public:
        texture(render_context& context, mge::texture_type type);
        texture(render_context&          context,
                mge::texture_type        type,
                const mge::image_format& format,
                const mge::extent&       extent,
                mge::texture_usage       usage);
        ~texture() override;

        void set_data(const mge::image_format& format,
                      const mge::extent&       extent,
                      const void*              data,
                      size_t                   size) override;

        GLuint texture_name() const noexcept
        {
            return m_texture;
        }

        bool is_depth_stencil() const noexcept
        {
            return m_image_format.format() ==
                   mge::image_format::data_format::DEPTH_STENCIL;
        }

    private:
        GLint  internal_format(const mge::image_format& format) const;
        GLenum pixel_format(const mge::image_format& format) const;
        GLenum pixel_type(const mge::image_format& format) const;

        GLuint            m_texture;
        mge::image_format m_image_format;
    };
} // namespace mge::opengl
