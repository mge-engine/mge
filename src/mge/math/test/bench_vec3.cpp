// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/math/vec3.hpp"

static void bench_fvec3_length(benchmark::State &state)
{

    std::vector<mge::fvec3> vec;
    mge::fvec3              v;
    v.x = 17.0f;
    v.y = 33.145f;
    v.z = 2.34f;
    for (uint32_t i = 0; i < 100000; ++i) {
        v.x += 0.017f;
        v.y -= 0.036f;
        v.z += 0.0621f;
        vec.push_back(v);
    }
    std::vector<float> lv;
    lv.resize(vec.size());
    while (state.KeepRunning()) {
        for (uint32_t i = 0; i < vec.size(); ++i) {
            lv[i] = mge::length(vec[i]);
        }
    }
}

BENCHMARK(bench_fvec3_length);

static void bench_fvec3_squared_length(benchmark::State &state)
{

    std::vector<mge::fvec3> vec;
    mge::fvec3              v;
    v.x = 17.0f;
    v.y = 33.145f;
    v.z = 2.34f;
    for (uint32_t i = 0; i < 100000; ++i) {
        v.x += 0.017f;
        v.y -= 0.036f;
        v.z += 0.0621f;
        vec.push_back(v);
    }
    std::vector<float> lv;
    lv.resize(vec.size());
    while (state.KeepRunning()) {
        for (uint32_t i = 0; i < vec.size(); ++i) {
            lv[i] = mge::squared_length(vec[i]);
        }
    }
}

BENCHMARK(bench_fvec3_squared_length);

static void bench_fvec3_length_manually(benchmark::State &state)
{

    std::vector<mge::fvec3> vec;
    mge::fvec3              v;
    v.x = 17.0f;
    v.y = 33.145f;
    v.z = 2.34f;
    for (uint32_t i = 0; i < 100000; ++i) {
        v.x += 0.017f;
        v.y -= 0.099f;
        v.z += 0.0621f;
        vec.push_back(v);
    }
    std::vector<float> lv;
    lv.resize(vec.size());
    while (state.KeepRunning()) {
        for (uint32_t i = 0; i < vec.size(); ++i) {
            lv[i] = std::sqrt(vec[i].x * vec[i].x + vec[i].y * vec[i].y +
                              vec[i].z * vec[i].z);
        }
    }
}

BENCHMARK(bench_fvec3_length_manually);
