// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/async_executor.hpp"

TEST(async_executor, sync_executor)
{
    bool executed = false;
    auto& executor = mge::async_executor::synchronous_executor();
    executor.await([&]{
        executed = true;
    });
    EXPECT_TRUE(executed);
}
