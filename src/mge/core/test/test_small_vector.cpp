// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/small_vector.hpp"
#include "test/googletest.hpp"

TEST(small_vector, default_ctor)
{
    mge::small_vector<int, 5> v;
    EXPECT_EQ(nullptr, v.data());
}

TEST(small_vector, ctor_with_default)
{
    mge::small_vector<int, 5> v1(3, 15);
    mge::small_vector<int, 5> v2(300, 15);
    mge::small_vector<int, 5> v3(0, 15);

    EXPECT_EQ(3u, v1.size());
    EXPECT_EQ(300u, v2.size());
    EXPECT_EQ(0u, v3.size());
}

TEST(small_vector, clear)
{
    mge::small_vector<int, 5> v1(3, 15);
    v1.clear();
    EXPECT_EQ(0u, v1.size());
}