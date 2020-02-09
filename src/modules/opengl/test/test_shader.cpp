// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "opengl_test.hpp"
#include "mge/graphics/window.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/render_context.hpp"

namespace opengl
{
class test_shader_program : public opengltest_with_window
{
};

TEST_F(test_shader_program, create)
{
    auto s = window->render_context()
        .create_shader_program(mge::shader_type::VERTEX);
}

TEST_F(test_shader_program, compile)
{
    const char *vertex_shader_glsl =
       "#version 330 core\n"
       "layout(location = 0) in vec3 vertexPosition;\n"
       "\n"
       "void main() {\n"
       "  gl_Position.xyz = vertexPosition;\n"
       "  gl_Position.w = 1.0;\n"
       "}";

    auto s = window->render_context()
        .create_shader_program(mge::shader_type::VERTEX);
    s->compile(vertex_shader_glsl);
    EXPECT_TRUE(s->defined());
}

TEST_F(test_shader_program, compile_with_syntax_error)
{
    const char *vertex_shader_glsl =
       "#version 330 core\n"
       "layout(location = 0) in vec3 vertexPosition;\n"
       "\n"
       "void main() {\n"
       "  gl_Position.xyz = vertexPosition;\n"
       "  gl_Position.w = 1.0;\n foobar"
       "}";

    auto s = window->render_context()
        .create_shader_program(mge::shader_type::VERTEX);
    try {
        s->compile(vertex_shader_glsl);
        FAIL() << "Compilation should throw";
    } catch(const mge::exception& e) {
        std::cerr << e;
    }
    EXPECT_FALSE(s->defined());
}


}
