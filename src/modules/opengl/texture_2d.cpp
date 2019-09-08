#include "texture_2d.hpp"
#include "error.hpp"
#include "common.hpp"

namespace opengl {
    texture_2d::texture_2d(mge::render_context& context,
                           mge::usage texture_usage,
                           mge::filter_function mag_filter,
                           mge::filter_function min_filter,
                           bool mipmap_use,
                           const mge::image_ref& image)
        :mge::texture_2d(context,
                         texture_usage,
                         mag_filter,
                         min_filter,
                         mipmap_use,
                         image)
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
