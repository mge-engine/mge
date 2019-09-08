#include "mge/graphics/texture_2d.hpp"
#include "opengl.hpp"

namespace opengl {
    class texture_2d : public mge::texture_2d
    {
    public:
        texture_2d(mge::render_context& context,
                    mge::usage texture_usage,
                    mge::filter_function mag_filter,
                    mge::filter_function min_filter,
                    bool mipmap_use,
                    const mge::image_ref& image);
        virtual ~texture_2d();
    private:
        GLuint m_texture;
    };
}
