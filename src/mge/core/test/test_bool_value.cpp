// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/bool_value.hpp"

TEST(bool_value, from_c_string_true)
{
    EXPECT_TRUE(mge::bool_value("true"));
    EXPECT_TRUE(mge::bool_value("yes"));
    EXPECT_TRUE(mge::bool_value("1"));
    EXPECT_TRUE(mge::bool_value("True"));
    EXPECT_TRUE(mge::bool_value("on"));
}

TEST(bool_value, from_c_string_false)
{
    EXPECT_FALSE(mge::bool_value("false"));
    EXPECT_FALSE(mge::bool_value("no"));
    EXPECT_FALSE(mge::bool_value("0"));
    EXPECT_FALSE(mge::bool_value("FALSE"));
    EXPECT_FALSE(mge::bool_value("off"));
}

TEST(bool_value, from_nullptr)
{
    EXPECT_FALSE(mge::bool_value(nullptr));
}

TEST(bool_value, from_string)
{
    std::string true_str("true");
    std::string false_str("false");

    EXPECT_TRUE(mge::bool_value(true_str));
    EXPECT_FALSE(mge::bool_value(false_str));
}
