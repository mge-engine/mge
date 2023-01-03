// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/mouse_state.hpp"
#include "test/googletest.hpp"

TEST(mouse_state, move)
{
    mge::mouse_state s;
    EXPECT_EQ(0u, s.x());
    EXPECT_EQ(0u, s.y());
    s.move(300u, 900u);
    EXPECT_EQ(300u, s.x());
    EXPECT_EQ(900u, s.y());
}
