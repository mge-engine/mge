#include "mge/graphics/texture_2d.hpp"
#include "opengl.hpp"

namespace opengl {
    class texture_2d : public mge::texture_2d
    {
    public:
        texture_2d(mge::render_context& context,
                    const mge::image_ref& image);
        texture_2d(mge::render_context& context);
        virtual ~texture_2d();
    private:
        GLuint m_texture;
    };
}
