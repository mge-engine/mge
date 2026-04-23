// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/viewport.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(viewport, default_constructor)
{
    mge::viewport v;
    EXPECT_FLOAT_EQ(0.0f, v.x);
    EXPECT_FLOAT_EQ(0.0f, v.y);
    EXPECT_FLOAT_EQ(0.0f, v.width);
    EXPECT_FLOAT_EQ(0.0f, v.height);
    EXPECT_FLOAT_EQ(0.0f, v.min_depth);
    EXPECT_FLOAT_EQ(0.0f, v.max_depth);
}

TEST(viewport, constructor)
{
    mge::viewport v(10.0f, 20.0f, 800.0f, 600.0f, 0.0f, 1.0f);
    EXPECT_FLOAT_EQ(10.0f, v.x);
    EXPECT_FLOAT_EQ(20.0f, v.y);
    EXPECT_FLOAT_EQ(800.0f, v.width);
    EXPECT_FLOAT_EQ(600.0f, v.height);
    EXPECT_FLOAT_EQ(0.0f, v.min_depth);
    EXPECT_FLOAT_EQ(1.0f, v.max_depth);
}

TEST(viewport, copy)
{
    mge::viewport v1(10.0f, 20.0f, 800.0f, 600.0f, 0.0f, 1.0f);
    mge::viewport v2(v1);
    EXPECT_FLOAT_EQ(v1.x, v2.x);
    EXPECT_FLOAT_EQ(v1.width, v2.width);
}

TEST(viewport, set_rect_floats)
{
    mge::viewport v;
    v.set_rect(5.0f, 10.0f, 640.0f, 480.0f);
    EXPECT_FLOAT_EQ(5.0f, v.x);
    EXPECT_FLOAT_EQ(10.0f, v.y);
    EXPECT_FLOAT_EQ(640.0f, v.width);
    EXPECT_FLOAT_EQ(480.0f, v.height);
}

TEST(viewport, set_rect_extent)
{
    mge::viewport v;
    mge::extent   ext(1024, 768);
    v.set_rect(ext);
    EXPECT_FLOAT_EQ(0.0f, v.x);
    EXPECT_FLOAT_EQ(0.0f, v.y);
    EXPECT_FLOAT_EQ(1024.0f, v.width);
    EXPECT_FLOAT_EQ(768.0f, v.height);
}

TEST(viewport, set_rect_rectangle)
{
    mge::viewport  v;
    mge::rectangle r(mge::point(10, 20), mge::extent(400, 300));
    v.set_rect(r);
    EXPECT_FLOAT_EQ(10.0f, v.x);
    EXPECT_FLOAT_EQ(20.0f, v.y);
    EXPECT_FLOAT_EQ(400.0f, v.width);
    EXPECT_FLOAT_EQ(300.0f, v.height);
}

TEST(viewport, data_pointer)
{
    mge::viewport v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
    const float*  d = v.data();
    EXPECT_FLOAT_EQ(1.0f, d[0]);
    EXPECT_FLOAT_EQ(2.0f, d[1]);
    EXPECT_FLOAT_EQ(3.0f, d[2]);
    EXPECT_FLOAT_EQ(4.0f, d[3]);
    EXPECT_FLOAT_EQ(5.0f, d[4]);
    EXPECT_FLOAT_EQ(6.0f, d[5]);
}

TEST(viewport, lower_left)
{
    mge::viewport v(10.0f, 20.0f, 100.0f, 50.0f, 0.0f, 1.0f);
    auto          ll = v.lower_left(600);
    EXPECT_EQ(10u, ll.x);
    EXPECT_EQ(530u, ll.y); // 600 - (20 + 50)
}

TEST(viewport, format)
{
    mge::viewport      v(10.0, 20.0, 30.0, 40.0, 0.0, 1.0);
    std::ostringstream oss;
    oss << v;
    EXPECT_EQ(
        "viewport{x=10, y=20, width=30, height=40, min_depth=0, max_depth=1}",
        oss.str());
}
