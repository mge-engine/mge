// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/entity/entity_registry.hpp"

static void bench_entity_system_create(benchmark::State &state)
{
    while (state.KeepRunning()) {
        mge::entity_registry r;
    }
}
BENCHMARK(bench_entity_system_create);

static void bench_entity_create(benchmark::State &state)
{
    mge::entity_registry r;
    while (state.KeepRunning()) {
        auto e = r.create();
    }
}
BENCHMARK(bench_entity_create);

static void bench_entity_create_destroy(benchmark::State &state)
{
    mge::entity_registry r;
    while (state.KeepRunning()) {
        auto e = r.create();
        r.destroy(e);
    }
}
BENCHMARK(bench_entity_create_destroy);

static void bench_entity_create_destroy_1000(benchmark::State &state)
{
    mge::entity          entities[1000];
    mge::entity_registry r;
    while (state.KeepRunning()) {
        for (int i = 0; i < 1000; ++i) {
            entities[i] = r.create();
        }
        for (int i = 0; i < 1000; ++i) {
            r.destroy(entities[i]);
        }
    }
}
BENCHMARK(bench_entity_create_destroy_1000);

static void bench_entity_create_destroy_1M(benchmark::State &state)
{
    std::vector<mge::entity> entities;
    entities.resize(1000000);
    mge::entity_registry r;
    while (state.KeepRunning()) {
        for (int i = 0; i < 1000000; ++i) {
            entities[i] = r.create();
        }
        for (int i = 0; i < 1000000; ++i) {
            r.destroy(entities[1000000 - i - 1]);
        }
    }
}
BENCHMARK(bench_entity_create_destroy_1M);

struct foo_info
{
    int x;
    int y;
};

static void bench_set_field_1M(benchmark::State &state)
{
    std::vector<mge::entity> entities;
    entities.resize(1000000);
    mge::entity_registry r;
    for (int i = 0; i < 1000000; ++i) {
        entities[i] = r.create();
    }
    while (state.KeepRunning()) {
        for (int i = 0; i < 1000000; ++i) {
            r.assign<foo_info>(entities[i], i, i);
        }
    }
}
BENCHMARK(bench_set_field_1M);

struct foo_object
{
    foo_info foo;
    foo_info space[1000];
};

static void bench_set_field_direct_1M(benchmark::State &state)
{
    std::vector<foo_object> entities;
    entities.resize(1000000);
    while (state.KeepRunning()) {
        for (int i = 0; i < 1000000; ++i) {
            entities[i].foo.x = i;
            entities[i].foo.y = i;
        }
    }
}
BENCHMARK(bench_set_field_direct_1M);
