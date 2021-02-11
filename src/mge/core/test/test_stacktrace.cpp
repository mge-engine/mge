// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stacktrace.hpp"
#include "test/googletest.hpp"

TEST(stacktrace, basic)
{
    auto stack = mge::stacktrace();
    EXPECT_EQ(5u, stack.size());
}
