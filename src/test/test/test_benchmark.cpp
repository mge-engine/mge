// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/benchmark.hpp"
#include "test/googletest.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

TEST(benchmark, memory_cycle)
{
    char* buffer = new char[10 * 1024];
    mge::benchmark()
        .minEpochIterations(1000000)
        .run("memset1",
             [&]() {
                 memset(buffer, 'A', 1 * 1024);
                 mge::do_not_optimize_away(buffer);
             })
        .run("memset10", [&]() {
            memset(buffer, 'A', 10 * 1024);
            mge::do_not_optimize_away(buffer);
        });
    delete[] buffer;
}


TEST(benchmark, throw_catch_cycle)
{
    mge::benchmark().run("throw_catch", [&]() {
        try {
            throw std::logic_error("test");
        } catch (const std::exception& e) {
            mge::do_not_optimize_away(e.what());
        }
    });
}
