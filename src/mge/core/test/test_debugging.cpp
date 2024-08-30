// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/debugging.hpp"
#include "test/googletest.hpp"

TEST(debugging, crash_if_no_debugger_present)
{
    EXPECT_DEATH(mge::breakpoint(),
                 "Calling debugger with no debugger attached");
}

TEST(debugging, is_debugger_present)
{
    EXPECT_FALSE(mge::is_debugger_present());
}

TEST(debugging, breakpoint_if_debugging) { mge::breakpoint_if_debugging(); }