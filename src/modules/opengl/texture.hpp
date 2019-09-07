#include "mge/graphics/texture.hpp"
#include "opengl.hpp"

namespace opengl {
    class texture : public mge::texture
    {
    public:
        texture(mge::render_context& context,
                mge::texture_type t,
                mge::usage texture_usage,
                mge::filter_function mag_filter,
                mge::filter_function min_filter,
                bool mipmap_use);
        virtual ~texture();
    private:
        GLuint m_texture;
    };
}
