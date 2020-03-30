// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/stacktrace.hpp"
#include "test/googletest.hpp"

#include <iostream>

using mge::stacktrace;

TEST(stacktrace, construct)
{
    stacktrace t;
    std::cout << t << std::endl;
    EXPECT_LE(5, t.size());
}

TEST(stacktrace, copy_and_assign)
{
    stacktrace t1;
    stacktrace t2(t1);

    EXPECT_EQ(t2, t1);
    stacktrace t3;
    t3 = t2;
    EXPECT_EQ(t2, t3);
}
