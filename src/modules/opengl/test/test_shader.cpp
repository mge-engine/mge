// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader.hpp"
#include "opengl_test.hpp"
#include "test/benchmark.hpp"

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
    context.frame();
    EXPECT_TRUE(shader->initialized());
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
        context.frame();
        FAIL() << "Expected exception";
    } catch (const mge::exception&) {
        // expected
    }
}

TEST_F(shader_test, set_code_failed)
{
    using namespace std::literals;

    auto& context = m_window->render_context();
    auto  shader = context.create_shader(mge::shader_type::VERTEX);

    auto            bogus = "bogus"sv;
    mge::buffer_ref code = mge::make_buffer(
        reinterpret_cast<const std::byte*>(bogus.data()),
        reinterpret_cast<const std::byte*>(bogus.data() + bogus.size()));
    try {
        shader->set_code(*code);
        context.frame();
        FAIL() << "Expected set_code to fail";
    } catch (const mge::exception&) {
        // expected
    }
}
#if 0
TEST_F(shader_test, bench_compile_successfully)
{
    using namespace std::literals;
    auto vertex_shader_glsl = "#version 330 core\n"
                              "layout(location = 0) in vec3 vertexPosition;\n"
                              "\n"
                              "void main() {\n"
                              "  gl_Position.xyz = vertexPosition;\n"
                              "  gl_Position.w = 1.0;\n"
                              "}"sv;

    auto& context = m_window->render_context();

    mge::benchmark().show_results().run("shader_compile", [&]() {
        auto shader = context.create_shader(mge::shader_type::VERTEX);
        shader->compile(vertex_shader_glsl);
    });
}
#endif