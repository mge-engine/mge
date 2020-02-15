// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/scene/scene.hpp"
#include "mge/scene/node.hpp"
#include "mge/math/transform.hpp"

static void bench_node_set_transform(benchmark::State& state)
{
    mge::scene sc;
    mge::fmat4 mat = mge::identity<mge::fmat4>();

    auto n = sc.create_node();
    while(state.KeepRunning()) {
        n.set_transform(mat);
        n.transform();
        n.clear_transform();
    }
}

BENCHMARK(bench_node_set_transform);
