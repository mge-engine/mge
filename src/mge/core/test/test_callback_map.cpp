// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/callback_map.hpp"
#include "test/googletest.hpp"

TEST(callback_map, functionality)
{
    mge::callback_map<void> callbacks;

    bool callback_called = false;
    auto f = [&]() { callback_called = true; };

    auto k1 = callbacks.insert(f);
    auto k2 = callbacks.insert(f); // can't distinguish
    EXPECT_NE(k1, k2);

    callbacks();
    EXPECT_TRUE(callback_called);

    callbacks.erase(12345u);
}