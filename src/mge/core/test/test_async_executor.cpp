// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/async_executor.hpp"
#include "test/googletest.hpp"

TEST(async_executor, sync_executor)
{
    bool  executed = false;
    auto &executor = mge::async_executor::synchronous_executor();
    executor.await([&] { executed = true; });
    EXPECT_TRUE(executed);
}
