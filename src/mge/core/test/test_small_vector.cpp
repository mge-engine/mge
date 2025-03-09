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

TEST(small_vector, pop_back)
{
    mge::small_vector<int, 5> v1(3, 15);
    EXPECT_EQ(3u, v1.size());
    EXPECT_EQ(15, v1.back());

    v1.pop_back();
    EXPECT_EQ(2u, v1.size());
    EXPECT_EQ(15, v1.back());

    v1.pop_back();
    EXPECT_EQ(1u, v1.size());
    EXPECT_EQ(15, v1.back());

    v1.pop_back();
    EXPECT_EQ(0u, v1.size());

    EXPECT_THROW(v1.pop_back(), std::out_of_range);
}

TEST(small_vector, initializer_list)
{
    // Empty initializer list
    mge::small_vector<int, 3> v1{};
    EXPECT_EQ(0u, v1.size());

    // Small storage
    mge::small_vector<int, 3> v2{1, 2};
    EXPECT_EQ(2u, v2.size());
    EXPECT_EQ(1, v2[0]);
    EXPECT_EQ(2, v2[1]);

    // Vector storage
    mge::small_vector<int, 3> v3{1, 2, 3, 4, 5};
    EXPECT_EQ(5u, v3.size());
    for (size_t i = 0; i < v3.size(); ++i) {
        EXPECT_EQ(i + 1, v3[i]);
    }

    // Single element
    mge::small_vector<int, 3> v4{42};
    EXPECT_EQ(1u, v4.size());
    EXPECT_EQ(42, v4[0]);
}