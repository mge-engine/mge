#include "texture.hpp"
#include "error.hpp"
#include "common.hpp"

namespace opengl {
    texture::texture(mge::render_context& context,
                     mge::texture_type t,
                     mge::usage texture_usage,
                     mge::filter_function mag_filter,
                     mge::filter_function min_filter,
                     bool mipmap_use)
        :mge::texture(context,
                      t,
                      texture_usage,
                      mag_filter,
                      min_filter,
                      mipmap_use)
        ,m_texture(0) 
    {
        await([&]{
            glGenTextures(1, &m_texture);
            CHECK_OPENGL_ERROR(glGenTextures);
        });
    }

    texture::~texture()
    {
        if(m_texture) {
            await([&]{
                glDeleteTextures(1, &m_texture);
                error::clear();
            });
        }
    }
}
