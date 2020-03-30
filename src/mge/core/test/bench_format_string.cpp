// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/format_string.hpp"

static void bench_format_string(benchmark::State &state)
{
    while (state.KeepRunning()) {
        std::string s(mge::format_string("foo", 1, 1.23, "bar"));
    }
}

BENCHMARK(bench_format_string);
