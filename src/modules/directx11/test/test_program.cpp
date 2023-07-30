// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/window.hpp"

class program_test : public mge::dx11::dx11test
{};

TEST_F(program_test, create_program)
{
    auto& context = m_window->render_context();
    auto  program = context.create_program();
    ASSERT_TRUE(program);
}

TEST_F(program_test, link_program)
{
    auto& context = m_window->render_context();
    auto  program = context.create_program();

    auto        vs = context.create_shader(mge::shader_type::VERTEX);
    const char* vertex_shader_hlsl =
        "uniform float translate_x;"
        "float4 main(float4 inPos : POSITION) : SV_POSITION\n"
        "{\n"
        "  return float4(inPos.x + translate_x, inPos.y, inPos.z, "
        "inPos.w);\n"
        "}";
    vs->compile(vertex_shader_hlsl);
    auto        fs = context.create_shader(mge::shader_type::FRAGMENT);
    const char* fragment_shader_hlsl = R"shader(
        float4 main() : SV_TARGET
        {
            return float4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    )shader";
    fs->compile(fragment_shader_hlsl);

    program->set_shader(vs);
    program->set_shader(fs);
    program->link();
}