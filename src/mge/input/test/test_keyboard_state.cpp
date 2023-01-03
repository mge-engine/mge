// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/keyboard_state.hpp"
#include "test/googletest.hpp"

TEST(keyboard_state, press_release)
{
    mge::keyboard_state s;
    EXPECT_FALSE(s.pressed(mge::key::KP_0));
    s.press(mge::key::KP_0);
    EXPECT_TRUE(s.pressed(mge::key::KP_0));
    s.press(mge::key::KP_1);
    EXPECT_TRUE(s.pressed(mge::key::KP_0));
    EXPECT_TRUE(s.pressed(mge::key::KP_1));
    s.release(mge::key::KP_0);
    EXPECT_FALSE(s.pressed(mge::key::KP_0));
    EXPECT_TRUE(s.pressed(mge::key::KP_1));
}
