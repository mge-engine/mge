// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/enum_set.hpp"
#include "test/googletest.hpp"

enum test_enum_set
{
    FOO = 0,
    FOO_BAR,
    FOO_FOO_BAR
};

TEST(enum_set, functionality)
{
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s;
    EXPECT_FALSE(s.test(FOO));
    s.set(FOO);
    EXPECT_TRUE(s.test(FOO));
    s.reset();
    EXPECT_FALSE(s.test(FOO));
    s.set(FOO_BAR);
    EXPECT_TRUE(s.test(FOO_BAR));
    s.flip(FOO_BAR);
    EXPECT_FALSE(s.test(FOO_BAR));
    s.set(FOO_BAR);
    EXPECT_TRUE(s.test(FOO_BAR));
    s.reset(FOO_BAR);
    EXPECT_FALSE(s.test(FOO_BAR));
}

TEST(enum_set, format)
{
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s;

    // empty set
    auto empty = fmt::format("{}", s);
    EXPECT_STREQ("", empty.c_str());

    // single value
    s.set(FOO);
    auto single = fmt::format("{}", s);
    EXPECT_STREQ("FOO", single.c_str());

    // multiple values
    s.set(FOO_BAR);
    auto multiple = fmt::format("{}", s);
    EXPECT_STREQ("FOO|FOO_BAR", multiple.c_str());
}