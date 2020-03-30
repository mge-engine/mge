// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/pipeline.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader_program.hpp"
#include "mge/graphics/window.hpp"
#include "opengl_test.hpp"
#include "test/test_exception.hpp"

namespace opengl {

    class test_pipeline : public opengltest_with_window
    {};

    TEST_F(test_pipeline, create)
    {
        auto pipeline = window->render_context().create_pipeline();
    }

    TEST_F(test_pipeline, link_empty)
    {
        auto pipeline = window->render_context().create_pipeline();
        EXPECT_THROW_WITH_MESSAGE(pipeline->link(), mge::exception,
                                  "glLinkProgram failed");
    }

    TEST_F(test_pipeline, successful_simple_link)
    {
        const char *vertex_shader_glsl =
            "#version 330 core\n"
            "layout(location = 0) in vec3 vertexPosition;\n"
            "\n"
            "void main() {\n"
            "  gl_Position.xyz = vertexPosition;\n"
            "  gl_Position.w = 1.0;\n"
            "}";
        const char *fragment_shader_glsl =
            "#version 330 core\n"
            "uniform vec3 fragmentColor = vec3(1.0, 1.0, 1.0);\n"
            "out vec3 color;\n"
            "\n"
            "void main() {\n"
            "    color = fragmentColor;\n"
            "}";
        auto vs = window->render_context().create_shader_program(
            mge::shader_type::VERTEX);
        vs->compile(vertex_shader_glsl);
        auto fs = window->render_context().create_shader_program(
            mge::shader_type::FRAGMENT);
        fs->compile(fragment_shader_glsl);
        auto p = window->render_context().create_pipeline();
        p->set_shader_program(vs);
        p->set_shader_program(fs);
        p->link();
        EXPECT_EQ(1u, p->attributes().size());
        EXPECT_EQ(std::string_view("vertexPosition"), p->attributes()[0].name);
        EXPECT_EQ(1u, p->attributes()[0].size);
    }

} // namespace opengl
