// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/is_container.hpp"
#include "test/googletest.hpp"
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>

template <typename T> bool is_container() { return mge::is_container<T>; }
template <typename T> bool is_sequence_container()
{
    return mge::is_sequence_container<T>;
}

TEST(concepts, is_container)
{
    EXPECT_TRUE(is_container<std::vector<int>>());
    EXPECT_TRUE(is_container<std::list<int>>());
    EXPECT_TRUE(is_container<std::deque<int>>());
    EXPECT_TRUE(is_container<std::forward_list<int>>());
    bool is_container_std_array = is_container<std::array<int, 3>>();
    EXPECT_TRUE(is_container_std_array);
    EXPECT_FALSE(is_container<int>());
}

TEST(concepts, is_sequence_container)
{
    EXPECT_TRUE(is_sequence_container<std::vector<int>>());
    EXPECT_TRUE(is_sequence_container<std::list<int>>());
    EXPECT_TRUE(is_sequence_container<std::deque<int>>());
    // forward_list does not support insert in container style
    EXPECT_FALSE(is_sequence_container<std::forward_list<int>>());
    bool is_sequence_container_std_array =
        is_sequence_container<std::array<int, 3>>();
    EXPECT_FALSE(is_sequence_container_std_array);
    EXPECT_FALSE(is_sequence_container<int>());
}

TEST(concepts, string_container)
{
    EXPECT_FALSE(is_sequence_container<std::string>());
    EXPECT_FALSE(is_sequence_container<std::wstring>());
    EXPECT_FALSE(is_sequence_container<std::string_view>());
    EXPECT_FALSE(is_sequence_container<std::wstring_view>());
}