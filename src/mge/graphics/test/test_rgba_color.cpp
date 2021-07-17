// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/rgb_color.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "test/googletest.hpp"

using namespace mge;

TEST(rgba_color, default_construct)
{
    rgba_color c;
    EXPECT_EQ(0.0f, c.r);
    EXPECT_EQ(0.0f, c.g);
    EXPECT_EQ(0.0f, c.b);
    EXPECT_EQ(0.0f, c.a);
}

TEST(rgba_color, from_rgb_color)
{
    rgb_color  c(0.1f, 0.2f, 0.3f);
    rgba_color ca(c, 1.0f);
    EXPECT_EQ(0.1f, ca.r);
    EXPECT_EQ(0.2f, ca.g);
    EXPECT_EQ(0.3f, ca.b);
    EXPECT_EQ(1.0f, ca.a);
}

TEST(rgba_color, equals)
{
    rgba_color c1(0.1f, 0.2f, 0.3f, 1.0f);
    rgba_color c2(0.1f, 0.2f, 0.3f, 1.0f);
    rgba_color c3(0.1f, 0.2f, 0.3f, 0.5f);
    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 == c3);
}

TEST(rgba_color, stream_output)
{
    rgba_color        c1(0.1f, 0.2f, 0.3f, 1.0f);
    std::stringstream ss;
    ss << c1;
    EXPECT_EQ(ss.str(), "rgba_color[r=0.1, g=0.2, b=0.3, a=1]");
}
