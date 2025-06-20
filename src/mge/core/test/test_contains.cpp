// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/contains.hpp"
#include "test/googletest.hpp"
#include <set>

TEST(contains, vector)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    EXPECT_TRUE(mge::contains(v, 3));
    EXPECT_FALSE(mge::contains(v, 6));
}

TEST(contains, small_vector)
{
    mge::small_vector<int, 5> sv = {1, 2, 3, 4, 5};
    EXPECT_TRUE(mge::contains(sv, 3));
    EXPECT_FALSE(mge::contains(sv, 6));
}

TEST(contains, array)
{
    std::array<int, 5> a = {1, 2, 3, 4, 5};
    EXPECT_TRUE(mge::contains(a, 3));
    EXPECT_FALSE(mge::contains(a, 6));
}

TEST(contains, set)
{
    std::set<int> s = {1, 2, 3, 4, 5};
    EXPECT_TRUE(mge::contains(s, 3));
    EXPECT_FALSE(mge::contains(s, 6));
}
