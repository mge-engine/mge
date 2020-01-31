// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/new.hpp"

static void bench_simple_malloc_free(benchmark::State& state)
{
    while (state.KeepRunning()) {
        for(size_t i=1; i<12345; ++i) {
            void *ptr = mge::malloc(i);
            mge::free(ptr);
        }
    }
}

BENCHMARK(bench_simple_malloc_free);

static void bench_simple_array_new_delete(benchmark::State& state)
{
    while (state.KeepRunning()) {
        for(size_t i=1; i<12345; ++i) {
            char *arrayptr = new char[i];
            delete[] arrayptr;
        }
    }
}

BENCHMARK(bench_simple_array_new_delete);

static void bench_simple_allocate_free(benchmark::State& state)
{
    while (state.KeepRunning()) {
        for(size_t i=1; i<12345; ++i) {
            void *ptr = mge::operator_new(i);
            mge::operator_delete(ptr);
        }
    }
}

BENCHMARK(bench_simple_allocate_free);

