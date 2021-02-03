// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/parameter.hpp"
#include "test/googletest.hpp"

using namespace std::string_view_literals;

MGE_DEFINE_PARAMETER(bool, test, test_parameter1, "a test parameter");

TEST(configuration, find_parameter)
{
    auto &p = mge::configuration::find_parameter("test"sv, "test_parameter1"sv);
    EXPECT_EQ("test_parameter1"sv, p.name());
}

TEST(configuration, find_parameter_throws_on_unknown)
{
    EXPECT_THROW(
        mge::configuration::find_parameter("test"sv, "unknown_parameter"sv),
        std::runtime_error);
}