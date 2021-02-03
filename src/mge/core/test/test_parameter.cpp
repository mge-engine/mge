// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/parameter.hpp"
#include "test/googletest.hpp"
using namespace std::string_view_literals;
MGE_DEFINE_PARAMETER(bool, test, test_parameter1, "a test parameter");

TEST(parameter, simple_access)
{
    EXPECT_EQ("test"sv, MGE_PARAMETER(test, test_parameter1).section());
    EXPECT_EQ("test_parameter1"sv, MGE_PARAMETER(test, test_parameter1).name());
    EXPECT_EQ("a test parameter"sv,
              MGE_PARAMETER(test, test_parameter1).description());
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter1).has_value());
}

TEST(parameter, from_string_get)
{
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter1).has_value());
    MGE_PARAMETER(test, test_parameter1).from_string("1");
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter1).has_value());
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter1).get());
    MGE_PARAMETER(test, test_parameter1).reset();
}

TEST(parameter, from_string_to_string)
{
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter1).has_value());
    MGE_PARAMETER(test, test_parameter1).from_string("1");
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter1).has_value());
    EXPECT_EQ(std::string("1"),
              MGE_PARAMETER(test, test_parameter1).to_string());
    MGE_PARAMETER(test, test_parameter1).reset();
}