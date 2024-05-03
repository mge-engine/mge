#pragma once
#include "mge/graphics/texture.hpp"
#include "opengl.hpp"

namespace mge::opengl {
    class render_context;

    class texture : public mge::texture
    {
    public:
        texture(render_context& context, mge::texture_type type);
        ~texture() override;

        void set_data(const mge::image_format& format,
                      const mge::extent&       extent,
                      const void*              data,
                      size_t                   size) override;

    private:
        GLint  internal_format(const mge::image_format& format) const;
        GLenum pixel_format(const mge::image_format& format) const;
        GLenum pixel_type(const mge::image_format& format) const;

        GLuint m_texture;
    };
} // namespace mge::opengl
