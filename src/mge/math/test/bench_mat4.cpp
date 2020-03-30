// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/math/mat4.hpp"
#include "mge/math/transform.hpp"
#include "mge/math/vec3.hpp"

static void bench_fmat4_determinant(benchmark::State &state)
{
    mge::fmat4 m =
        mge::translate(mge::fmat4(1.0f), mge::fvec3(1.0f, 2.0f, 3.0f));
    while (state.KeepRunning()) {
        mge::determinant(m);
    }
}

BENCHMARK(bench_fmat4_determinant);

static void bench_fmat4_transpose(benchmark::State &state)
{
    mge::fmat4 m =
        mge::translate(mge::fmat4(1.0f), mge::fvec3(1.0f, 2.0f, 3.0f));
    while (state.KeepRunning()) {
        mge::transpose(m);
    }
}

BENCHMARK(bench_fmat4_transpose);

static void bench_fmat4_mult(benchmark::State &state)
{
    mge::fmat4 m1 =
        mge::translate(mge::fmat4(1.0f), mge::fvec3(1.0f, 2.0f, 3.0f));
    mge::fmat4 m2 =
        mge::translate(mge::fmat4(1.0f), mge::fvec3(1.0f, 2.0f, 3.0f));
    while (state.KeepRunning()) {
        auto m3 = m1 * m2;
    }
}

BENCHMARK(bench_fmat4_mult);

static void bench_fmat4_inverse(benchmark::State &state)
{
    mge::fmat4 m = mge::scale(mge::fmat4(1.0), mge::fvec3(3.0f));
    while (state.KeepRunning()) {
        auto inv_m = mge::inverse(m);
    }
}

BENCHMARK(bench_fmat4_inverse);
