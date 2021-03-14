// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/video_mode.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(video_mode, default_ctor)
{
    mge::video_mode m;
    mge::extent     ext;
    EXPECT_EQ(ext, m.extent);
}

TEST(video_mode, output)
{
    mge::video_mode m;

    m.extent       = mge::extent(800, 600);
    m.refresh_rate = 60;

    mge::test_stream_output(m, "800x600@60");
}