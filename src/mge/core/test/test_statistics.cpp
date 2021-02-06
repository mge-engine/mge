// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/statistics.hpp"
#include "test/googletest.hpp"

using namespace std::string_view_literals;

TEST(statistics, create)
{
    mge::statistics s("foobar");

    EXPECT_EQ("foobar"sv, s.name());
    s.release();
}

TEST(statistics, create_from_string)
{
    mge::statistics s(std::string("foobar"));

    EXPECT_EQ("foobar"sv, s.name());
    s.release();
}

TEST(statistics, root_exists)
{
    EXPECT_EQ(""sv, mge::statistics::root().name());
}

TEST(statistics, death_on_destroy_owned)
{
    ASSERT_DEATH({ mge::statistics s("foobar"); }, ".*");
}