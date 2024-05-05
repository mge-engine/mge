// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/make_string_view.hpp"
#include "test/googletest.hpp"

TEST(make_string_view, empty)
{
    auto sv = mge::make_string_view("");
    EXPECT_EQ(0, sv.size());
    EXPECT_NE(nullptr, sv.data());
}

TEST(make_string_view, non_empty)
{
    auto sv = mge::make_string_view("Hello");
    EXPECT_EQ(5, sv.size());
    EXPECT_EQ('H', sv[0]);
    EXPECT_EQ('e', sv[1]);
    EXPECT_EQ('l', sv[2]);
    EXPECT_EQ('l', sv[3]);
    EXPECT_EQ('o', sv[4]);
}

TEST(make_string_view, single_char)
{
    char a[1] = {'H'};
    auto sv = mge::make_string_view(a);
    EXPECT_EQ(1, sv.size());
    EXPECT_EQ('H', sv[0]);
}
