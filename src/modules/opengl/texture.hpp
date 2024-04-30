#pragma once
#include "mge/graphics/texture.hpp"
#include "opengl.hpp"

namespace mge::opengl {
    class render_context;

    class texture : mge::texture
    {
    public:
        texture(render_context& context, mge::texture_type type);
        ~texture() override;

        void set_data(mge::image_format  format,
                      const mge::extent& extent,
                      const void*        data,
                      size_t             size) override;

    private:
        GLuint m_texture;
    };
} // namespace mge::opengl
