// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "win32/monitor.hpp"
#include "mge/graphics/monitor.hpp"

TEST(monitor, all_monitors)
{
    auto m = mge::win32::monitor::all_monitors();
    EXPECT_FALSE(m.empty());
}

TEST(monitor, all_monitors_contains_primary)
{
    auto ms = mge::win32::monitor::all_monitors();
    bool primary_found = false;
    for(const auto& m : ms) {
        if(m->primary()) {
            primary_found = true;
        }
    }
    EXPECT_TRUE(primary_found) << "Primary monitor not found";
}

TEST(monitor, gamma_ramp)
{
    mge::gamma_ramp ramp;
    auto ms = mge::win32::monitor::all_monitors();
    for(const auto& m : ms) {
        if(m->primary()) {
            ramp = m->gamma_ramp();
            break;
        }
    }
    EXPECT_TRUE(ramp.size() >= 256);
}

TEST(monitor, video_modes)
{
    auto ms = mge::win32::monitor::all_monitors();
    ASSERT_FALSE(ms.empty());
    for(const auto& m : ms) {
        auto modes = m->video_modes();
        EXPECT_TRUE(modes.size() >= 1);
    }
}

TEST(monitor, current_mode)
{
    auto m = mge::win32::monitor::primary_monitor();
    auto mode = m->current_video_mode();
    EXPECT_LT(640, mode.width);
    EXPECT_LT(480, mode.height);
}
