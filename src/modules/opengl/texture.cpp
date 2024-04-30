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

    void texture::set_data(mge::image_format  format,
                           const mge::extent& extent,
                           const void*        data,
                           size_t             size)
    {
        if (type() == mge::texture_type::TYPE_2D) {
            glBindTexture(GL_TEXTURE_2D, m_texture);
            CHECK_OPENGL_ERROR();
        } else {
            MGE_THROW(mge::not_yet_implemented)
                << "Texture type " << type() << " not implemented";
        }
    }
} // namespace mge::opengl