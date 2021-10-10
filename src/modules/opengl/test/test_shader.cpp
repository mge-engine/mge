// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader.hpp"
#include "opengl_test.hpp"

class shader_test : public mge::opengl::opengltest
{};

TEST_F(shader_test, create)
{
    auto& context = m_window->render_context();
    auto  shader = context.create_shader(mge::shader_type::VERTEX);
    EXPECT_TRUE(shader);
}

TEST_F(shader_test, compile_successfully)
{
    using namespace std::literals;

    auto& context = m_window->render_context();
    auto  shader = context.create_shader(mge::shader_type::VERTEX);

    auto vertex_shader_glsl = "#version 330 core\n"
                              "layout(location = 0) in vec3 vertexPosition;\n"
                              "\n"
                              "void main() {\n"
                              "  gl_Position.xyz = vertexPosition;\n"
                              "  gl_Position.w = 1.0;\n"
                              "}"sv;

    shader->compile(vertex_shader_glsl);
}

TEST_F(shader_test, compile_failed)
{
    using namespace std::literals;

    auto& context = m_window->render_context();
    auto  shader = context.create_shader(mge::shader_type::VERTEX);

    auto vertex_shader_glsl = "#version 330 core\n"
                              "layout(location = 0) in vec3 vertexPosition;\n"
                              "\n"
                              "void main() {\n"
                              "  gl_Position.xyz = vertexPosition;\n"
                              "  gl_Position.w = 1.0;\n"
                              ""sv;

    try {
        shader->compile(vertex_shader_glsl);
        FAIL() << "Expected exception";
    } catch (const mge::exception&) {
        // expected
    }
}
