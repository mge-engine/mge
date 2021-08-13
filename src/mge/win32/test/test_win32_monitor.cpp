// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/contains.hpp"
#include "mge/win32/monitor.hpp"
#include "test/googletest.hpp"
#include <iostream>

TEST(monitor, all_monitors)
{
    auto all_monitors = mge::win32::monitor::all_monitors();
    EXPECT_GE(all_monitors.size(), 1u);
}

TEST(monitor, all_monitors_0_primary)
{
    auto all_monitors = mge::win32::monitor::all_monitors();
    EXPECT_TRUE(all_monitors.at(0)->primary());
}

TEST(monitor, supported_modes)
{
    auto mon = mge::win32::monitor::all_monitors().at(0);
    auto modes = mon->supported_modes();
    EXPECT_GE(modes.size(), 1u);
}

TEST(monitor, physical_size)
{
    auto mon = mge::win32::monitor::all_monitors().at(0);
    EXPECT_GE(mon->physical_size().width, 0u);
    EXPECT_GE(mon->physical_size().height, 0u);
}

TEST(monitor, current_mode_in_supported)
{
    auto mon = mge::win32::monitor::all_monitors().at(0);
    auto mode = mon->current_mode();
    auto modes = mon->supported_modes();
    EXPECT_TRUE(mge::contains(modes, mode));
}