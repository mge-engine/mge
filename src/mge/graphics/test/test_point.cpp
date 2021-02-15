// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/point.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(point, default_ctor)
{
    mge::point d;
    EXPECT_EQ(0u, d.x);
    EXPECT_EQ(0u, d.y);
}

TEST(point, ctor)
{
    mge::point d(640u, 480u);
    EXPECT_EQ(640u, d.x);
    EXPECT_EQ(480u, d.y);
}

TEST(point, stream_op)
{
    mge::point d(640u, 480u);
    mge::test_stream_output(d, "point{x=640, y=480}");
}
