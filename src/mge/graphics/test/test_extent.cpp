// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/extent.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(extent, default_ctor)
{
    mge::extent d;
    EXPECT_EQ(0u, d.width);
    EXPECT_EQ(0u, d.height);
}

TEST(extent, ctor)
{
    mge::extent d(640u, 480u);
    EXPECT_EQ(640u, d.width);
    EXPECT_EQ(480u, d.height);
}

TEST(extent, stream_op)
{
    mge::extent d(640u, 480u);
    mge::test_stream_output(d, "extent{width=640, height=480}");
}
