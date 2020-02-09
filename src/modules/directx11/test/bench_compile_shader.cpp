// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"
#include "bench/googlebenchmark.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader.hpp"

namespace dx11 {

static void bench_compile_shader(benchmark::State& state)
{
    dx11test_with_window::SetUpTestCase();
    while(state.KeepRunning()) {
        const char *vertex_shader_hlsl =
        "uniform float translate_x;"
        "float4 main(float4 inPos : POSITION) : SV_POSITION\n"
        "{\n"
        "  return float4(inPos.x + translate_x, inPos.y, inPos.z, inPos.w);\n"
        "}";
        auto s = dx11test_with_window::window->render_context()
            .create_shader_program(mge::shader_type::VERTEX);
        s->compile(vertex_shader_hlsl);
    }
    dx11test_with_window::TearDownTestCase();
}

BENCHMARK(bench_compile_shader);




}
