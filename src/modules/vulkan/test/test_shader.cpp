// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader.hpp"
#include "vulkan_test.hpp"

class shader_test : public mge::vulkan::vulkantest
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
    auto  vertex_shader_glsl = "#version 330 core\n"
                               "layout(location = 0) in vec3 vertexPosition;\n"
                               "\n"
                               "void main() {\n"
                               "  gl_Position.xyz = vertexPosition;\n"
                               "  gl_Position.w = 1.0;\n"
                               "}"sv;
    EXPECT_FALSE(shader->initialized());
    shader->compile(vertex_shader_glsl);
    context.frame();
    EXPECT_TRUE(shader->initialized());
    shader.destroy();
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
    } catch (const mge::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    EXPECT_FALSE(shader->initialized());
    shader.destroy();
}
