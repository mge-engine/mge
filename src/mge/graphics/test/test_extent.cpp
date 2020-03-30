// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/extent.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(extent, print)
{
    mge::extent ex(640, 480);
    mge::test_stream_output(ex, "extent[width=640, height=480]");
}

TEST(extent, area)
{
    mge::extent ex(640, 480);
    EXPECT_EQ(640 * 480, ex.area());
}
