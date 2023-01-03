// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/call_debugger.hpp"
#include "test/googletest.hpp"

TEST(call_debugger, crash_if_no_debugger_present)
{
    EXPECT_DEATH(mge::call_debugger(),
                 "Calling debugger with no debugger attached");
}