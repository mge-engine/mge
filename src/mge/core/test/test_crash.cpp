// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/crash.hpp"

TEST(crash, crash_terminates_program)
{
    ASSERT_DEATH(mge::crash(), "");
}


TEST(crash, crash_with_message)
{
    ASSERT_DEATH(mge::crash("foobar"), "foobar");
}


TEST(crash, crash_with_streamed_message)
{
    ASSERT_DEATH(mge::crash("foo","bar"), "foobar");
}
