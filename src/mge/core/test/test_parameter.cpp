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
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter1).has_value());
}
