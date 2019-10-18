// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/math/vec3.hpp"
#include "mge/math/mat4.hpp"
#include "mge/math/transform.hpp"

static void bench_look_at(benchmark::State& state)
{
    using namespace mge;

    fvec3 eye{1.0, 1.0, 1.0};
    fvec3 center{0,0,0};
    fvec3 up{0,1,0};
    fvec3 inc{0.001, 0.001, 0.001};
    

    while (state.KeepRunning()) {
        auto m = look_at(eye, center, up);
        eye += inc;
    }
}

BENCHMARK(bench_look_at);
