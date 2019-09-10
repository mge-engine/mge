#include "opengl_test.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/texture_2d.hpp"

namespace opengl {
    class test_texture : public opengltest_with_window
    {};

    TEST_F(test_texture, create)
    {
        auto texture = window->render_context()
            .create_texture_2d();
        EXPECT_TRUE(texture.get() != nullptr);
    }
}
