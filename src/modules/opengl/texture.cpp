// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "texture.hpp"
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "render_context.hpp"

namespace mge::opengl {

    texture::texture(render_context& context, mge::texture_type type)
        : mge::texture(context, type)
        , m_texture(0)
    {
        glGenTextures(1, &m_texture);
    }

    texture::~texture()
    {
        if (m_texture) {
            glDeleteTextures(1, &m_texture);
        }
    }

    GLint texture::internal_format(const mge::image_format& format) const
    {
        switch (format.format()) {
        case mge::image_format::data_format::RGB:
            return GL_RGB;
        case mge::image_format::data_format::RGBA:
            return GL_RGBA;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unsuppprted image format (internal format): " << format;
        }
    }

    GLenum texture::pixel_format(const mge::image_format& format) const
    {
        switch (format.format()) {
        case mge::image_format::data_format::RGB:
            return GL_RGB;
        case mge::image_format::data_format::RGBA:
            return GL_RGBA;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unsupported image format (pixel format): " << format;
        }
    }

    GLenum texture::pixel_type(const mge::image_format& format) const
    {
        switch (format.type()) {
        case mge::data_type::INT8:
            return GL_BYTE;
        case mge::data_type::UINT8:
            return GL_UNSIGNED_BYTE;
        case mge::data_type::INT16:
            return GL_SHORT;
        case mge::data_type::UINT16:
            return GL_UNSIGNED_SHORT;
        case mge::data_type::INT32:
            return GL_INT;
        case mge::data_type::UINT32:
            return GL_UNSIGNED_INT;
        case mge::data_type::FLOAT:
            return GL_FLOAT;
        case mge::data_type::DOUBLE:
            return GL_DOUBLE;
        case mge::data_type::HALF:
            return GL_HALF_FLOAT;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unsupported image type (pixel type): " << format;
        }
    }

    void texture::set_data(const mge::image_format& format,
                           const mge::extent&       extent,
                           const void*              data,
                           size_t                   size)
    {
        if (type() == mge::texture_type::TYPE_2D) {
            // Image data is top-to-bottom (stb_image convention).
            // Not flipping: OpenGL places row 0 at texture bottom,
            // so V=0 maps to image top, matching DX/Vulkan convention.
            glBindTexture(GL_TEXTURE_2D, m_texture);
            CHECK_OPENGL_ERROR(glBindTexture);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         internal_format(format),
                         extent.width,
                         extent.height,
                         0,
                         pixel_format(format),
                         pixel_type(format),
                         data);
            CHECK_OPENGL_ERROR(glTexImage2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            CHECK_OPENGL_ERROR(glTexParameteri);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            CHECK_OPENGL_ERROR(glTexParameteri);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            CHECK_OPENGL_ERROR(glTexParameteri);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            CHECK_OPENGL_ERROR(glTexParameteri);
            glBindTexture(GL_TEXTURE_2D, 0);
            CHECK_OPENGL_ERROR(glBindTexture(0));
        } else {
            MGE_THROW(mge::not_yet_implemented)
                << "Texture type " << type() << " not implemented";
        }
    }
} // namespace mge::opengl