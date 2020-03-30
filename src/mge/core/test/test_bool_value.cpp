// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/bool_value.hpp"
#include "test/googletest.hpp"
#include <string_view>

using namespace std::string_view_literals;

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

TEST(bool_value, from_nullptr) { EXPECT_FALSE(mge::bool_value(nullptr)); }

TEST(bool_value, from_string)
{
    std::string true_str("true");
    std::string false_str("false");

    EXPECT_TRUE(mge::bool_value(true_str));
    EXPECT_FALSE(mge::bool_value(false_str));
}

TEST(bool_value, from_string_view_true)
{
    EXPECT_TRUE(mge::bool_value("true"sv));
    EXPECT_TRUE(mge::bool_value("yes"sv));
    EXPECT_TRUE(mge::bool_value("1"sv));
    EXPECT_TRUE(mge::bool_value("True"sv));
    EXPECT_TRUE(mge::bool_value("on"sv));
    EXPECT_TRUE(mge::bool_value("0111"sv));
}

TEST(bool_value, from_string_view_false)
{
    EXPECT_FALSE(mge::bool_value("false"sv));
    EXPECT_FALSE(mge::bool_value("no"sv));
    EXPECT_FALSE(mge::bool_value("0"sv));
    EXPECT_FALSE(mge::bool_value("FALSE"sv));
    EXPECT_FALSE(mge::bool_value("off"sv));
}
