#include "texture_2d.hpp"
#include "error.hpp"
#include "common.hpp"

namespace opengl {
    texture_2d::texture_2d(mge::render_context& context,
                           const mge::image_ref& image)
        :mge::texture_2d(context,
                         image)
        ,m_texture(0)
    {
        await([&]{
            glGenTextures(1, &m_texture);
            CHECK_OPENGL_ERROR(glGenTextures);
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
