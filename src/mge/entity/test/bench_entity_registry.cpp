// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/entity/entity_registry.hpp"

static void bench_entity_system_create(benchmark::State& state)
{
    while(state.KeepRunning()) {
        mge::entity_registry r;
    }
}
BENCHMARK(bench_entity_system_create);

static void bench_entity_create(benchmark::State& state)
{
    mge::entity_registry r;
    while(state.KeepRunning()) {
        auto e = r.create();
    }
}
BENCHMARK(bench_entity_create);

static void bench_entity_create_destroy(benchmark::State& state)
{
    mge::entity_registry r;
    while(state.KeepRunning()) {
        auto e = r.create();
        r.destroy(e);
    }
}
BENCHMARK(bench_entity_create_destroy);