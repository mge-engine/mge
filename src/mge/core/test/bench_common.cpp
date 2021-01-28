// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log.hpp"
#include "test/benchmark.hpp"
#include "test/googletest.hpp"
#include <random>

template <typename T> T add(T first) { return first; }

template <typename T, typename... Args> T add(T first, Args... args)
{
    return first + add(args...);
}

template <typename T> int addf(T &&first) { return first(); }

template <typename T, typename... Args>
MGE_NO_INLINE int addf(T &&first, Args &&... args)
{
    return first() + addf(args...);
}

TEST(benchmark, pass_integers)
{
    std::mt19937 rng;
    mge::benchmark()
        .show_results()
        .run("pass 5 random integers",
             [&]() {
                 int x = add(rng(), rng(), rng(), rng(), rng());
                 mge::do_not_optimize_away(x);
             })
        .run("pass 5 functions", [&]() {
            int x = 0;
            for (int i = 1; i < 1000; ++i) {
                x +=
                    addf(MGE_WRAP_IN_CLOSURE(rng()), MGE_WRAP_IN_CLOSURE(rng()),
                         MGE_WRAP_IN_CLOSURE(rng()), MGE_WRAP_IN_CLOSURE(rng()),
                         MGE_WRAP_IN_CLOSURE(rng()), MGE_WRAP_IN_CLOSURE(rng()),
                         MGE_WRAP_IN_CLOSURE(rng()), MGE_WRAP_IN_CLOSURE(rng()),
                         MGE_WRAP_IN_CLOSURE(rng()), MGE_WRAP_IN_CLOSURE(rng())

                    );
            }
            mge::do_not_optimize_away(x);
        });
}