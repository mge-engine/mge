// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "texture_2d.hpp"
#include "error.hpp"
#include "common.hpp"
#include "mge/core/stdexceptions.hpp"

namespace opengl {

    static GLint gl_internal_format(mge::image_format f)
    {
        switch(f) {
        case mge::image_format::RGBA8:
            return GL_RGBA;
        case mge::image_format::RGB8:
            return GL_RGB;
        default:
            MGE_THROW(mge::illegal_argument) << "Unsupported image format " << f;
        }
    }

    texture_2d::texture_2d(mge::render_context& context,
                           const mge::image_ref& image)
        :mge::texture_2d(context,
                         image)
        ,m_texture(0)
    {
        if (!image->mapped()) {
            MGE_THROW(mge::illegal_argument) << "Image is not mapped";
        }
        await([&]{
            glGenTextures(1, &m_texture);
            CHECK_OPENGL_ERROR(glGenTextures);
            glBindTexture(GL_TEXTURE_2D, m_texture);
            CHECK_OPENGL_ERROR(glBindTexture);
            glTexImage2D(GL_TEXTURE_2D,
                         0, /* level */
                         gl_internal_format(image->format()),
                         m_extent.width(),
                         m_extent.height(),
                         0, /* border */
                         gl_internal_format(image->format()),
                         GL_UNSIGNED_BYTE,
                         image->data());
            CHECK_OPENGL_ERROR(glTexImage2D);
        });
    }

    texture_2d::texture_2d(mge::render_context& context)
        :mge::texture_2d(context)
        ,m_texture(0)
    {
        await([&]{
            glGenTextures(1, &m_texture);
            CHECK_OPENGL_ERROR(glGenTextures);
        });
    }


    texture_2d::~texture_2d()
    {
        if(m_texture) {
            await([&]{
                glDeleteTextures(1, &m_texture);
                error::clear();
            });
        }
    }
}
