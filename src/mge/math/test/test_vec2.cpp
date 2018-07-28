// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/math/vec2.hpp"
#include "mge/core/stdexceptions.hpp"

using namespace mge;

TEST(vec2, at)
{
    vec2f v(0.0f, 1.0f);
    v.at(0) = 1.0f;
    v.at(1) = 3.0f;
    EXPECT_EQ(1.0f, v.at(0));
    EXPECT_EQ(3.0f, v.at(1));
}

TEST(vec2, at_const)
{
    const vec2f v(1.0f, 3.0f);
    EXPECT_EQ(1.0f, v.at(0));
    EXPECT_EQ(3.0f, v.at(1));
}

TEST(vec2, at_out_of_range)
{
    vec2f v(0.0f, 1.0f);
    EXPECT_THROW(v.at(17) = 1.0f, mge::out_of_range);
}

TEST(vec2, const_at_out_of_range)
{
    const vec2f v(0.0f, 1.0f);
    EXPECT_THROW(v.at(17), mge::out_of_range);
}

TEST(vec2, abs)
{
    vec2f v1(1.0f, -10.0f);
    vec2f v2(1.0f, 10.0f);
    EXPECT_EQ(v2, abs(v1));
}

TEST(vec2, add_assign)
{
    vec2f v(1.0f, 2.0f);
    v += 3.0f;
    EXPECT_EQ(v.x, 4.0f);
    EXPECT_EQ(v.y, 5.0f);
}


