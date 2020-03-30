// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "opengl_test.hpp"
namespace opengl {
    class test_shader_language : public opengltest_with_window
    {};

    TEST_F(test_shader_language, test_shader_language_returns_value)
    {
        std::vector<mge::shader_language> l;
        window->render_context().shader_languages(l);
        EXPECT_GE(l.size(), 1u);
    }

    TEST_F(test_shader_language, test_shader_language_is_glsl)
    {
        std::vector<mge::shader_language> l;
        window->render_context().shader_languages(l);
        EXPECT_STREQ("glsl", l[0].type().c_str());
    }

    TEST_F(test_shader_language, test_shader_language_is_greater_3)
    {
        std::vector<mge::shader_language> l;
        window->render_context().shader_languages(l);
        EXPECT_GE(l[0].version(), mge::version("3"));
    }

} // namespace opengl
