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

TEST(enum_set, copy_constructor)
{
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s1;
    s1.set(FOO);
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s2(s1);
    EXPECT_TRUE(s2.test(FOO));
    EXPECT_FALSE(s2.test(FOO_BAR));
}

TEST(enum_set, move_constructor)
{
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s1;
    s1.set(FOO_BAR);
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s2(std::move(s1));
    EXPECT_TRUE(s2.test(FOO_BAR));
}

TEST(enum_set, assignment)
{
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s1;
    s1.set(FOO);
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s2;
    s2 = s1;
    EXPECT_TRUE(s2.test(FOO));
}

TEST(enum_set, empty)
{
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s;
    EXPECT_TRUE(s.empty());
    s.set(FOO);
    EXPECT_FALSE(s.empty());
}

TEST(enum_set, reset_other)
{
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s1;
    s1.set(FOO);
    s1.set(FOO_BAR);

    mge::enum_set<test_enum_set, FOO_FOO_BAR> mask;
    mask.set(FOO);

    s1.reset(mask);
    EXPECT_FALSE(s1.test(FOO));
    EXPECT_TRUE(s1.test(FOO_BAR));
}

TEST(enum_set, includes)
{
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s1;
    s1.set(FOO);
    s1.set(FOO_BAR);

    mge::enum_set<test_enum_set, FOO_FOO_BAR> s2;
    s2.set(FOO);

    EXPECT_TRUE(s1.includes(s2));
    EXPECT_FALSE(s2.includes(s1));
}

TEST(enum_set, or_equals)
{
    mge::enum_set<test_enum_set, FOO_FOO_BAR> s1;
    s1.set(FOO);

    mge::enum_set<test_enum_set, FOO_FOO_BAR> s2;
    s2.set(FOO_BAR);

    s1 |= s2;
    EXPECT_TRUE(s1.test(FOO));
    EXPECT_TRUE(s1.test(FOO_BAR));
}