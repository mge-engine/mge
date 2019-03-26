// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/exception.hpp"

static void bench_mge_throw(benchmark::State& state)
{
    while (state.KeepRunning()) {
        try {
            MGE_THROW(mge::exception);
        } catch(const mge::exception&) {
            // noop
        }
    }
}

static void bench_mge_throw_with_message(benchmark::State& state)
{
    while (state.KeepRunning()) {
        try {
            MGE_THROW(mge::exception) << "Some message" << " for the exception";
        } catch(const mge::exception&) {
            // noop
        }
    }
}

static void bench_mge_throw_and_catch(benchmark::State& state)
{
    while (state.KeepRunning()) {
        try {
            MGE_THROW(mge::exception) << "Some message" << " for the exception";
        } catch(const mge::exception& ex) {
            std::stringstream ss;
            ss << ex.details();
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


static void bench_exception_message_construction(benchmark::State& state)
{
    while (state.KeepRunning()) {

        mge::exception ex;
        ex << "Some " << " message " << " composed";
        ex.what();
    }
}




BENCHMARK(bench_mge_throw);
BENCHMARK(bench_mge_throw_and_catch);
BENCHMARK(bench_mge_throw_with_message);
BENCHMARK(bench_exception_message_construction);
BENCHMARK(bench_unaugmented_throw);
