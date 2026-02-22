// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/contains.hpp"
#include "test/googletest.hpp"

#include <vector>
#include <list>
#include <set>

TEST(contains, vector_int)
{
    std::vector<int> v{1, 2, 3, 4, 5};
    EXPECT_TRUE(mge::contains(v, 3));
    EXPECT_FALSE(mge::contains(v, 6));
}

TEST(contains, list_string)
{
    std::list<std::string> l{"apple", "banana", "cherry"};
    EXPECT_TRUE(mge::contains(l, std::string("banana")));
    EXPECT_FALSE(mge::contains(l, std::string("date")));
}

TEST(contains, set_double)
{
    std::set<double> s{1.1, 2.2, 3.3};
    EXPECT_TRUE(mge::contains(s, 2.2));
    EXPECT_FALSE(mge::contains(s, 4.4));
}

TEST(contains, array_char)
{
    std::array<char, 5> a{'a', 'b', 'c', 'd', 'e'};
    EXPECT_TRUE(mge::contains(a, 'c'));
    EXPECT_FALSE(mge::contains(a, 'z'));
}
