// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/format.hpp"
#include "mge/math/vec2.hpp"
#include "test/googletest.hpp"
#include <sstream>

using namespace mge;

TEST(vec2, abs)
{
    fvec2 v1(1.0f, -10.0f);
    fvec2 v2(1.0f, 10.0f);
    EXPECT_EQ(v2, abs(v1));
}

TEST(vec2, add_assign)
{
    fvec2 v(1.0f, 2.0f);
    v += 3.0f;
    EXPECT_EQ(v.x, 4.0f);
    EXPECT_EQ(v.y, 5.0f);
}

TEST(vec2, minus)
{
    fvec2 v1(1.0f, 2.0f);
    fvec2 v2(-v1);
    fvec2 v3(-v2);
    EXPECT_EQ(v3, v1);
}

TEST(vec2, squared_length)
{
    fvec2 v(3.0f, 4.0f);
    EXPECT_EQ(squared_length(v), 25.0f);
}

TEST(vec2, print)
{
    fvec2             v(1.0f, 2.0f);
    std::stringstream ss;
    ss << v;
    EXPECT_STREQ("(1, 2)", ss.str().c_str());
}

TEST(ivec2, print)
{
    ivec2             v(1, 2);
    std::stringstream ss;
    ss << v;
    EXPECT_STREQ("(1, 2)", ss.str().c_str());
}

TEST(vec2, format)
{
    EXPECT_EQ("(1, 2)", fmt::format("{}", fvec2(1.0f, 2.0f)));
    EXPECT_EQ("(1, 2)", fmt::format("{}", ivec2(1, 2)));
    EXPECT_EQ("(1, 2)", fmt::format("{}", uvec2(1u, 2u)));
    EXPECT_EQ("(1, 2)", fmt::format("{}", dvec2(1.0, 2.0)));
}
