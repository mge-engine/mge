// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"
#include "mge/core/version.hpp"

using namespace mge;

TEST(version, construct)
{
    version v;
    EXPECT_EQ(0u, v.major());
    EXPECT_EQ(0u, v.minor());
    EXPECT_EQ(0u, v.patch());
}

TEST(version, construct_components)
{
    version v(8u, 7u, 6u);
    EXPECT_EQ(8u, v.major());
    EXPECT_EQ(7u, v.minor());
    EXPECT_EQ(6u, v.patch());
}

TEST(version, construct_string)
{
    version v("8.7.6");
    EXPECT_EQ(8u, v.major());
    EXPECT_EQ(7u, v.minor());
    EXPECT_EQ(6u, v.patch());
}

TEST(version, compare)
{
    version v1("1.2.1");
    version v2("1.3.1");

    EXPECT_TRUE(v1 < v2);
    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v2 == v2);
    EXPECT_TRUE(v1 != v2);
    EXPECT_TRUE(v2 > v1);
    EXPECT_TRUE(v2 >= v1);
}

TEST(version, stream)
{
    version v1("1");
    version v2("1.2");
    version v3("1.2.3");

    mge::test_stream_output(v1, "1");
    mge::test_stream_output(v2, "1.2");
    mge::test_stream_output(v3, "1.2.3");
}
