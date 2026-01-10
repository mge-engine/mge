// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/contains.hpp"
#include "mge/win32/monitor.hpp"
#include "test/googletest.hpp"
#include <iostream>

TEST(monitor, all_monitors)
{
    auto all_monitors = mge::win32::monitor::all_monitors();
    EXPECT_GE(all_monitors.size(), 1u);
    std::cout << "Detected monitors: " << all_monitors.size() << "\n";
}

TEST(monitor, all_monitors_0_primary)
{
    auto all_monitors = mge::win32::monitor::all_monitors();
    EXPECT_TRUE((*all_monitors.begin())->primary());
}

TEST(monitor, supported_modes)
{
    auto mon = *(mge::win32::monitor::all_monitors().begin());
    auto modes = mon->supported_modes();
    EXPECT_GE(modes.size(), 1u);
    std::cout << "Supported modes: " << modes.size() << "\n";
}

TEST(monitor, physical_size)
{
    auto mon = *(mge::win32::monitor::all_monitors().begin());
    EXPECT_GE(mon->physical_size().width, 0u);
    EXPECT_GE(mon->physical_size().height, 0u);
}

TEST(monitor, current_mode_in_supported)
{
    auto mon = *(mge::win32::monitor::all_monitors().begin());
    auto mode = mon->current_mode();
    auto modes = mon->supported_modes();
    EXPECT_TRUE(mge::contains(modes, mode));
}