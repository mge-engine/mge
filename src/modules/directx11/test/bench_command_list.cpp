#include "dx11_test.hpp"
#include "bench/googlebenchmark.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/command_list.hpp"

namespace dx11 {

static void bench_command_list_create_destroy(benchmark::State& state)
{
    dx11test_with_window::SetUpTestCase();
    while(state.KeepRunning()) {
        //auto l = dx11test_with_window::window->render_context().create_command_list();
    }
    dx11test_with_window::TearDownTestCase();
}

BENCHMARK(bench_command_list_create_destroy);




}