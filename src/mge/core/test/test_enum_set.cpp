// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/enum_set.hpp"
#include "test/googletest.hpp"

enum class test_enum
{
    ENUM_A,
    ENUM_B,
    ENUM_C,
};

TEST(enum_set, construct)
{
    mge::enum_set<test_enum, test_enum::ENUM_C> s;
    EXPECT_TRUE(s.empty());
}

TEST(enum_set, set_test)
{
    mge::enum_set<test_enum, test_enum::ENUM_C> s;
    s.set(test_enum::ENUM_B);
    EXPECT_FALSE(s.empty());
    EXPECT_TRUE(s.test(test_enum::ENUM_B));
}

TEST(enum_set, reset)
{
    mge::enum_set<test_enum, test_enum::ENUM_C> s;
    s.set(test_enum::ENUM_B);
    EXPECT_FALSE(s.empty());
    s.reset();
    EXPECT_TRUE(s.empty());
}
