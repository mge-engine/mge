// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "dx11_test.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/render_context.hpp"

namespace dx11 {

    static void bench_command_list_create_destroy(benchmark::State &state)
    {
        dx11test_with_window::SetUpTestCase();
        while (state.KeepRunning()) {
            auto l = dx11test_with_window::window->render_context()
                         .create_command_list();
        }
        dx11test_with_window::TearDownTestCase();
    }

    BENCHMARK(bench_command_list_create_destroy);

} // namespace dx11