#include "opengl_test.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/texture.hpp"

namespace opengl {
    class test_texture : public opengltest_with_window
    {};

    TEST_F(test_texture, create)
    {
        auto texture = window->render_context()
            .create_texture(mge::texture_type::TEXTURE_2D,
                            mge::usage::DEFAULT,
                            mge::filter_function::NEAREST,
                            mge::filter_function::NEAREST,
                            false);
    }
}
