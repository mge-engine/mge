// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/entity/entity_registry.hpp"
#include "mge/entity/entity_registry.hpp"
#include "mge/core/tagged.hpp"
#include "mge/math/vec3.hpp"

static void bench_entity_assign_get_component(benchmark::State& state)
{
    mge::entity_registry r;
    auto e = r.create();

    struct position_tag {};
    struct look_at_tag {};

    using position = mge::tagged<mge::fvec3, position_tag>;
    mge::fvec3 v(3.0, 7.0, 9.0);
    while(state.KeepRunning()) {
        r.assign_or_replace<position>(e, v);
        v = r.get<position>(e);
        v.x += 0.001;
        v.y += 0.001;
        v.z += 0.001;
    }
}

BENCHMARK(bench_entity_assign_get_component);
