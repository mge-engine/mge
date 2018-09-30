// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/exception.hpp"

static void bench_mge_throw(benchmark::State& state)
{
    while (state.KeepRunning()) {
        try {
            MGE_THROW(mge::exception());
        } catch(const mge::exception&) {
            // noop
        }
    }
}

static void bench_unaugmented_throw(benchmark::State& state)
{
    while (state.KeepRunning()) {
        try {
            throw mge::exception();
        } catch(const mge::exception&) {
            // noop
        }
    }
}


BENCHMARK(bench_mge_throw);
BENCHMARK(bench_unaugmented_throw);
