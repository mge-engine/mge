// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/rgb_color.hpp"
#include "test/googletest.hpp"

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
    rgb_color         c("orange");
    std::stringstream ss;
    ss << c;
    EXPECT_EQ(ss.str(), std::string("rgb_color{r=1, g=0.64705884, b=0}"));
}
