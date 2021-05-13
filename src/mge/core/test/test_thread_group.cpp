// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/thread_group.hpp"
#include "test/googletest.hpp"

TEST(thread_group, construct)
{
    auto tg = std::make_shared<mge::thread_group>();
    EXPECT_EQ(0u, tg->size());
}