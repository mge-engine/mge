// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/new.hpp"

static void bench_simple_malloc_free(benchmark::State& state)
{
    while (state.KeepRunning()) {
        void *ptr = mge::malloc(12345);
        mge::free(ptr);
    }
}

BENCHMARK(bench_simple_malloc_free);

static void bench_simple_array_new_delete(benchmark::State& state)
{
    while (state.KeepRunning()) {
        char *arrayptr = new char[12345];
        delete[] arrayptr;
    }
}

BENCHMARK(bench_simple_array_new_delete);

