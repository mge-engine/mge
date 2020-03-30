// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/clock.hpp"
#include "test/googletest.hpp"
#include <thread>

TEST(clock, returns_increasing_result)
{
    auto c1 = mge::clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto c2 = mge::clock::now();
    EXPECT_GT(c2, c1);
}

TEST(clock, measures_in_nanos)
{
    auto c1 = mge::clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto c2   = mge::clock::now();
    auto diff = c2 - c1;
    EXPECT_LE(490000000u, diff);
}
