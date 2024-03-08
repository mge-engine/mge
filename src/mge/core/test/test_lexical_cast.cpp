// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/lexical_cast.hpp"
#include "test/googletest.hpp"

using namespace std::string_view_literals;

TEST(lexical_cast, string_to_int)
{
    EXPECT_EQ(42, mge::lexical_cast<int>("42"));
}

TEST(lexical_cast, string_view_to_bool)
{
    EXPECT_TRUE(mge::lexical_cast<bool>("true"sv));
    EXPECT_FALSE(mge::lexical_cast<bool>("false"sv));
    EXPECT_TRUE(mge::lexical_cast<bool>("1"sv));
    EXPECT_FALSE(mge::lexical_cast<bool>("0"sv));
}