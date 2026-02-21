// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/glfw/monitor.hpp"
#include "test/googletest.hpp"
#include <iostream>

TEST(monitor, all_monitors)
{
    auto all = mge::glfw::monitor::all_monitors();
    EXPECT_GE(all.size(), 1u);
    std::cout << "Detected monitors: " << all.size() << "\n";
}

TEST(monitor, first_is_primary)
{
    auto all = mge::glfw::monitor::all_monitors();
    bool has_primary = false;
    for (const auto& m : all) {
        if (m->primary()) {
            has_primary = true;
            break;
        }
    }
    EXPECT_TRUE(has_primary);
}

TEST(monitor, supported_modes)
{
    auto all = mge::glfw::monitor::all_monitors();
    auto modes = all[0]->supported_modes();
    EXPECT_GE(modes.size(), 1u);
    std::cout << "Supported modes: " << modes.size() << "\n";
}

TEST(monitor, physical_size)
{
    auto all = mge::glfw::monitor::all_monitors();
    auto size = all[0]->physical_size();
    EXPECT_GE(size.width, 0u);
    EXPECT_GE(size.height, 0u);
}

TEST(monitor, current_mode_set)
{
    auto all = mge::glfw::monitor::all_monitors();
    auto mode = all[0]->current_mode();
    EXPECT_GT(mode.extent.width, 0u);
    EXPECT_GT(mode.extent.height, 0u);
    EXPECT_GT(mode.refresh_rate, 0u);
}
