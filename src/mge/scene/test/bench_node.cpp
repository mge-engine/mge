// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/math/transform.hpp"
#include "mge/scene/node.hpp"
#include "mge/scene/scene.hpp"

#if 0
static void bench_node_set_transform(benchmark::State& state)
{
    mge::scene sc;
    mge::fmat4 mat = mge::identity<mge::fmat4>();

    auto n = sc.create_node();
    while(state.KeepRunning()) {
        n.set_transform(mat);
        n.transform();
    }
}

BENCHMARK(bench_node_set_transform);
#endif

static void bench_node_set_position(benchmark::State &state)
{
    mge::scene sc;
    mge::fvec3 pos;

    auto n = sc.create_node();
    while (state.KeepRunning()) {
        pos = n.position();
        pos += 0.01;
        n.set_position(pos);
    }
}

BENCHMARK(bench_node_set_position);
