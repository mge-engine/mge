// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/types.hpp"
#include "test/googletest.hpp"
#include <string>

using namespace std::string_literals;

TEST(stringify_macro, list_of_args)
{
    std::string s(MGE_STRINGIFY(A, B, C));
    EXPECT_EQ("A, B, C"s, s);
}

TEST(stringify_macro, single_arg)
{
    std::string s(MGE_STRINGIFY(A));
    EXPECT_EQ("A"s, s);
}
