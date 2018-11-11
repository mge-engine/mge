/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "test/googletest.hpp"
#include "win32/monitor.hpp"
#include "mge/graphics/monitor.hpp"

TEST(monitor, all_monitors)
{
    auto m = win32::monitor::all_monitors();
    EXPECT_FALSE(m.empty());
}

TEST(monitor, all_monitors_contains_primary)
{
    auto ms = win32::monitor::all_monitors();
    bool primary_found = false;
    for(const auto& m : ms) {
        if(m->primary()) {
            primary_found = true;
        }
    }
    EXPECT_TRUE(primary_found) << "Primary monitor not found";
}
#if 0
TEST(monitor, gamma_ramp)
{
    mge::gamma_ramp ramp;
    std::vector<mge::monitor_ref> ms;
    win32::monitor::all_monitors(ms);
    for(const auto& m : ms) {
        if(m->primary()) {
            ramp = m->gamma_ramp();
            break;
        }
    }
    EXPECT_TRUE(ramp.size() >= 256);
}
#endif

#if 0
TEST(monitor, video_modes)
{
    auto ms = win32::monitor::all_monitors();
    ASSERT_FALSE(ms.empty());
    for(const auto& m : ms) {
        std::vector<mge::video_mode> modes;
        m->video_modes(modes);
        EXPECT_TRUE(modes.size() >= 1);
    }
}
#endif
