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

TEST(small_vector, pmr_small_vector)
{
    std::pmr::monotonic_buffer_resource  resource;
    std::pmr::polymorphic_allocator<int> alloc(&resource);

    mge::pmr_small_vector<int, 3> v1(alloc);
    EXPECT_EQ(0u, v1.size());

    v1.push_back(1);
    v1.push_back(2);
    EXPECT_EQ(2u, v1.size());
    EXPECT_EQ(1, v1[0]);
    EXPECT_EQ(2, v1[1]);

    // Overflow to vector, should use the pmr allocator
    v1.push_back(3);
    v1.push_back(4);
    EXPECT_EQ(4u, v1.size());
    EXPECT_EQ(4, v1[3]);
}

TEST(small_vector, pmr_small_vector_sized_ctor)
{
    std::pmr::monotonic_buffer_resource  resource;
    std::pmr::polymorphic_allocator<int> alloc(&resource);

    mge::pmr_small_vector<int, 3> v(5, 42, alloc);
    EXPECT_EQ(5u, v.size());
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(42, v[i]);
    }
}

TEST(small_vector, pmr_small_vector_initializer_list)
{
    std::pmr::monotonic_buffer_resource  resource;
    std::pmr::polymorphic_allocator<int> alloc(&resource);

    mge::pmr_small_vector<int, 2> v({10, 20, 30}, alloc);
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(10, v[0]);
    EXPECT_EQ(20, v[1]);
    EXPECT_EQ(30, v[2]);
}

TEST(small_vector, copy_constructor)
{
    mge::small_vector<int, 3> v1{1, 2, 3};
    mge::small_vector<int, 3> v2(v1);
    EXPECT_EQ(3u, v2.size());
    EXPECT_EQ(1, v2[0]);
    EXPECT_EQ(2, v2[1]);
    EXPECT_EQ(3, v2[2]);
}

TEST(small_vector, move_constructor)
{
    mge::small_vector<int, 3> v1{1, 2, 3};
    mge::small_vector<int, 3> v2(std::move(v1));
    EXPECT_EQ(3u, v2.size());
    EXPECT_EQ(1, v2[0]);
}

TEST(small_vector, copy_assignment)
{
    mge::small_vector<int, 3> v1{1, 2};
    mge::small_vector<int, 3> v2;
    v2 = v1;
    EXPECT_EQ(2u, v2.size());
    EXPECT_EQ(1, v2[0]);
    EXPECT_EQ(2, v2[1]);
}

TEST(small_vector, move_assignment)
{
    mge::small_vector<int, 3> v1{1, 2};
    mge::small_vector<int, 3> v2;
    v2 = std::move(v1);
    EXPECT_EQ(2u, v2.size());
    EXPECT_EQ(1, v2[0]);
}

TEST(small_vector, front)
{
    mge::small_vector<int, 3> v{42, 10};
    EXPECT_EQ(42, v.front());
}

TEST(small_vector, back)
{
    mge::small_vector<int, 3> v{42, 10};
    EXPECT_EQ(10, v.back());
}

TEST(small_vector, at)
{
    mge::small_vector<int, 3> v{1, 2, 3};
    EXPECT_EQ(2, v.at(1));
    EXPECT_THROW(v.at(10), std::out_of_range);
}

TEST(small_vector, at_empty)
{
    mge::small_vector<int, 3> v;
    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST(small_vector, data)
{
    mge::small_vector<int, 3> v{5, 6};
    EXPECT_NE(nullptr, v.data());
    EXPECT_EQ(5, v.data()[0]);
    EXPECT_EQ(6, v.data()[1]);
}

TEST(small_vector, empty)
{
    mge::small_vector<int, 3> v;
    EXPECT_TRUE(v.empty());
    v.push_back(1);
    EXPECT_FALSE(v.empty());
}

TEST(small_vector, iterators)
{
    mge::small_vector<int, 3> v{1, 2, 3};
    int                       sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(6, sum);
}

TEST(small_vector, const_iterators)
{
    const mge::small_vector<int, 3> v{1, 2, 3};
    int                             sum = 0;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(6, sum);
}

TEST(small_vector, reverse_iterators)
{
    mge::small_vector<int, 4> v{1, 2, 3};
    std::vector<int>          reversed;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        reversed.push_back(*it);
    }
    ASSERT_EQ(3u, reversed.size());
    EXPECT_EQ(3, reversed[0]);
    EXPECT_EQ(2, reversed[1]);
    EXPECT_EQ(1, reversed[2]);
}

TEST(small_vector, emplace_back)
{
    mge::small_vector<int, 3> v;
    v.emplace_back(42);
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ(42, v[0]);
}

TEST(small_vector, emplace_back_overflow)
{
    mge::small_vector<int, 2> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3); // overflow to vector
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(3, v[2]);
}

TEST(small_vector, insert_at_end)
{
    mge::small_vector<int, 5> v{1, 2};
    std::vector<int>          extra{3, 4};
    v.insert(v.cend(), extra.begin(), extra.end());
    EXPECT_EQ(4u, v.size());
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(4, v[3]);
}

TEST(small_vector, resize_grow)
{
    mge::small_vector<int, 5> v{1, 2};
    v.resize(4);
    EXPECT_EQ(4u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
}

TEST(small_vector, resize_shrink)
{
    mge::small_vector<int, 5> v{1, 2, 3, 4};
    v.resize(2);
    EXPECT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
}

TEST(small_vector, resize_to_zero)
{
    mge::small_vector<int, 5> v{1, 2, 3};
    v.resize(0);
    EXPECT_EQ(0u, v.size());
}

TEST(small_vector, resize_with_fill)
{
    mge::small_vector<int, 5> v;
    v.resize(3, 99);
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(99, v[0]);
    EXPECT_EQ(99, v[2]);
}

TEST(small_vector, equality)
{
    mge::small_vector<int, 3> v1{1, 2, 3};
    mge::small_vector<int, 3> v2{1, 2, 3};
    EXPECT_EQ(v1, v2);
}

TEST(small_vector, inequality)
{
    mge::small_vector<int, 3> v1{1, 2, 3};
    mge::small_vector<int, 3> v2{1, 2, 4};
    EXPECT_NE(v1, v2);
}

TEST(small_vector, inequality_size)
{
    mge::small_vector<int, 3> v1{1, 2};
    mge::small_vector<int, 3> v2{1, 2, 3};
    EXPECT_NE(v1, v2);
}

TEST(small_vector, swap)
{
    mge::small_vector<int, 3> v1{1, 2};
    mge::small_vector<int, 3> v2{3, 4, 5};
    v1.swap(v2);
    EXPECT_EQ(3u, v1.size());
    EXPECT_EQ(2u, v2.size());
}

TEST(small_vector, capacity_small)
{
    mge::small_vector<int, 5> v{1, 2};
    EXPECT_GE(v.capacity(), 2u);
}

TEST(small_vector, push_back_move)
{
    mge::small_vector<std::string, 2> v;
    std::string                       s("hello");
    v.push_back(std::move(s));
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ("hello", v[0]);
}