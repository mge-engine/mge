// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/math/vec3.hpp"
#include "test/googletest.hpp"

using namespace mge;

TEST(vec3, abs)
{
    fvec3 v1(1.0f, -10.0f, -45.0f);
    fvec3 v2(1.0f, 10.0f, 45.0f);
    EXPECT_EQ(v2, abs(v1));
}

TEST(vec3, add_assign)
{
    fvec3 v(1.0f, 2.0f, 5.0f);
    v += 3.0f;
    EXPECT_EQ(v.x, 4.0f);
    EXPECT_EQ(v.y, 5.0f);
    EXPECT_EQ(v.z, 8.0f);
}

TEST(vec3, minus)
{
    fvec3 v1(1.0f, 2.0f, 4.0f);
    fvec3 v2(-v1);
    fvec3 v3(-v2);
    EXPECT_EQ(v3, v1);
}

TEST(vec3, print)
{
    fvec3             v(1.0f, 2.0f, 3.0f);
    std::stringstream ss;
    ss << v;
    EXPECT_STREQ("[1, 2, 3]", ss.str().c_str());
}
