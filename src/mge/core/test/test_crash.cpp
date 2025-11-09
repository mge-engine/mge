// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include "test/googletest.hpp"

TEST(crash, crashes)
{
    EXPECT_DEATH(mge::crash(), "Crash!!!");
}

TEST(crash, crashes_with_message)
{

    EXPECT_DEATH(mge::crash("Crash %d %d %d", 1, 2, 3), "Crash 1 2 3");
}
