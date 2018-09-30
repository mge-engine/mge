// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/stacktrace.hpp"

static void bench_stacktrace(benchmark::State& state)
{
    while (state.KeepRunning()) {
        mge::stacktrace bt;
    }
}
BENCHMARK(bench_stacktrace);
