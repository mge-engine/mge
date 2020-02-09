// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/window.hpp"
#include "mge/graphics/render_context.hpp"

namespace dx11
{

class test_shader_program : public dx11test_with_window
{
};

TEST_F(test_shader_program, create)
{
    auto s = window->render_context().create_shader_program(mge::shader_type::VERTEX);
    EXPECT_TRUE(s.get() != nullptr);
}

TEST_F(test_shader_program, compile)
{
    const char *vertex_shader_hlsl =
        "uniform float translate_x;"
        "float4 main(float4 inPos : POSITION) : SV_POSITION\n"
        "{\n"
        "  return float4(inPos.x + translate_x, inPos.y, inPos.z, inPos.w);\n"
        "}";
    auto s = window->render_context()
        .create_shader_program(mge::shader_type::VERTEX);
    s->compile(vertex_shader_hlsl);
    EXPECT_TRUE(s->defined());
}

TEST_F(test_shader_program, compile_with_syntax_error)
{
    const char *vertex_shader_hlsl =
        "float4 main(float4 inPos : POSITION) : SV_POSITION\n"
        "{\n foobar"
        "  return inPos;\n"
        "}";

    auto s = window->render_context()
        .create_shader_program(mge::shader_type::VERTEX);
    
    try {
        s->compile(vertex_shader_hlsl);
        FAIL() << "Compilation should throw";
    } catch(const mge::exception& e) {
        std::cout << e;
    }
    EXPECT_FALSE(s->defined());
}

}
