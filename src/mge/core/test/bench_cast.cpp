// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/checked_cast.hpp"

static void bench_mge_checked_cast_unchecked(benchmark::State &state)
{
    size_t       source = 0;
    unsigned int target = 0;
    while (state.KeepRunning()) {
        ++source;
        target = (unsigned int)source;
    }
}

static void bench_mge_checked_cast_checked(benchmark::State &state)
{
    size_t       source = 0;
    unsigned int target = 0;
    while (state.KeepRunning()) {
        ++source;
        target = mge::checked_cast<unsigned int>(source);
    }
}

BENCHMARK(bench_mge_checked_cast_unchecked);
BENCHMARK(bench_mge_checked_cast_checked);