// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include "test/googletest.hpp"

TEST(crash, crash_terminates_program) { ASSERT_DEATH(mge::crash(), ""); }

TEST(crash, crash_with_message) { ASSERT_DEATH(mge::crash("foobar"), ""); }

TEST(crash, crash_with_streamed_message)
{
    ASSERT_DEATH(mge::crash("foo", "bar"), "");
}
