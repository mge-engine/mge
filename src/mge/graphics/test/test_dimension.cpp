// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/dimension.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(dimension, default_ctor)
{
    mge::dimension d;
    EXPECT_EQ(0u, d.width);
    EXPECT_EQ(0u, d.height);
}

TEST(dimension, ctor)
{
    mge::dimension d(640u, 480u);
    EXPECT_EQ(640u, d.width);
    EXPECT_EQ(480u, d.height);
}

TEST(dimension, stream_op)
{
    mge::dimension d(640u, 480u);
    mge::test_stream_output(d, "dimension{width=640, height=480}");
}
