// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/rgb_color.hpp"

using namespace mge;

TEST(rgb_color, default_construct)
{
    rgb_color c;
    EXPECT_EQ(0.0f, c.r);
    EXPECT_EQ(0.0f, c.g);
    EXPECT_EQ(0.0f, c.b);
}

TEST(rgb_color, from_string)
{
    rgb_color c("orange");
}

TEST(rgb_color, stream_output)
{
    rgb_color c("orange");
    std::stringstream ss;
    ss << c;
    EXPECT_EQ(ss.str(), std::string("rgb_color[r=1, g=0.647059, b=0]"));
}
