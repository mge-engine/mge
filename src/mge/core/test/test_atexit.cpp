// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/atexit.hpp"
#include "mge/core/crash.hpp"
#include "test/googletest.hpp"
#include <iostream>

TEST(atexit, atexit)
{
    mge::atexit::run([]() { std::cerr << "atexit was called" << std::endl; });
    EXPECT_DEATH(::exit(1), "atexit was called");
}

// On Linux, global destructors run after abort signal processing,
// so this test is only meaningful on Windows.
#ifdef MGE_OS_WINDOWS
TEST(atexit, atexit_called_in_crash)
{
    mge::atexit::run(
        []() { std::cerr << "atexit called in crash" << std::endl; });
    EXPECT_DEATH(mge::crash("atexit in crash"), "atexit called in crash");
}
#endif