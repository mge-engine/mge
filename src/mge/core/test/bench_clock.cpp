// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/clock.hpp"

static void bench_clock_now(benchmark::State &state)
{
    while (state.KeepRunning()) {
        auto n = mge::clock::now();
        MGE_UNUSED(n);
    }
}

BENCHMARK(bench_clock_now);

static void bench_clock_tick(benchmark::State &state)
{
    while (state.KeepRunning()) {
        auto n = mge::clock::tick();
        MGE_UNUSED(n);
    }
}

BENCHMARK(bench_clock_tick);
