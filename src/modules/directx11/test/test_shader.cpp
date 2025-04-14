// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/window.hpp"
#include "test/benchmark.hpp"

class shader_test : public mge::dx11::dx11test
{};

TEST_F(shader_test, create)
{
    auto& context = m_window->render_context();
    auto  s = context.create_shader(mge::shader_type::VERTEX);
    EXPECT_TRUE(s.get() != nullptr);
}

TEST_F(shader_test, compile)
{
    const char* vertex_shader_hlsl =
        "uniform float translate_x;"
        "float4 main(float4 inPos : POSITION) : SV_POSITION\n"
        "{\n"
        "  return float4(inPos.x + translate_x, inPos.y, inPos.z, "
        "inPos.w);\n"
        "}";
    auto& context = m_window->render_context();
    auto  s = context.create_shader(mge::shader_type::VERTEX);
    s->compile(vertex_shader_hlsl);
    EXPECT_TRUE(s->initialized());
}

TEST_F(shader_test, bench_compile)
{
    const char* vertex_shader_hlsl =
        "uniform float translate_x;"
        "float4 main(float4 inPos : POSITION) : SV_POSITION\n"
        "{\n"
        "  return float4(inPos.x + translate_x, inPos.y, inPos.z, "
        "inPos.w);\n"
        "}";
    auto& context = m_window->render_context();
    mge::benchmark().run("shader_compile", [&]() {
        auto s = context.create_shader(mge::shader_type::VERTEX);
        s->compile(vertex_shader_hlsl);
    });
}

TEST_F(shader_test, compile_with_syntax_error)
{
    const char* vertex_shader_hlsl =
        "float4 main(float4 inPos : POSITION) : SV_POSITION\n"
        "{\n foobar"
        "  return inPos;\n"
        "}";

    auto& context = m_window->render_context();
    auto  s = context.create_shader(mge::shader_type::VERTEX);

    try {
        s->compile(vertex_shader_hlsl);
        FAIL() << "Compilation should throw";
    } catch (const mge::exception& e) {
        std::cout << e;
    }
    EXPECT_FALSE(s->initialized());
}
