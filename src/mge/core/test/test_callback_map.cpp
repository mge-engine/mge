// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/callback_map.hpp"
#include "test/googletest.hpp"

TEST(callback_map, functionality)
{
    mge::callback_map<void()> callbacks;

    bool callback_called = false;
    auto f               = [&]() { callback_called = true; };

    auto k1 = callbacks.insert(f);
    EXPECT_FALSE(callbacks.empty());
    auto k2 = callbacks.insert(f); // can't distinguish
    EXPECT_NE(k1, k2);

    auto f1 = callbacks.erase(k1);
    f1();
    EXPECT_TRUE(callback_called);

    auto f2 = callbacks.erase(12345u);
    EXPECT_FALSE(f2);
}