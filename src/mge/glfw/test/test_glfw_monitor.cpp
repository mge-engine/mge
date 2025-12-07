// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/contains.hpp"
#include "mge/glfw/monitor.hpp"
#include "test/googletest.hpp"
#include <iostream>

TEST(glfw_monitor, all_monitors)
{
    auto all_monitors = mge::glfw::monitor::all_monitors();
    EXPECT_GE(all_monitors.size(), 1u);
}

TEST(glfw_monitor, all_monitors_0_primary)
{
    auto all_monitors = mge::glfw::monitor::all_monitors();
    EXPECT_TRUE((*all_monitors.begin())->primary());
}

TEST(glfw_monitor, supported_modes)
{
    auto mon = *(mge::glfw::monitor::all_monitors().begin());
    auto modes = mon->supported_modes();
    EXPECT_GE(modes.size(), 1u);
}

TEST(glfw_monitor, physical_size)
{
    auto mon = *(mge::glfw::monitor::all_monitors().begin());
    EXPECT_GE(mon->physical_size().width, 0u);
    EXPECT_GE(mon->physical_size().height, 0u);
}

TEST(glfw_monitor, current_mode_in_supported)
{
    auto mon = *(mge::glfw::monitor::all_monitors().begin());
    auto mode = mon->current_mode();
    auto modes = mon->supported_modes();
    EXPECT_TRUE(mge::contains(modes, mode));
}
