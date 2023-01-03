// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/string_pool.hpp"
#include "test/googletest.hpp"

using namespace std::string_view_literals;

TEST(string_pool, create)
{
    mge::string_pool p;
    EXPECT_EQ(0u, p.size());
}

TEST(string_pool, get_char_pointer)
{
    mge::string_pool p;

    auto sv1 = p.get("wurst");
    auto sv2 = p.get("wurst");

    EXPECT_EQ(sv1.begin(), sv2.begin());
    EXPECT_EQ(sv1.size(), sv2.size());
}

TEST(string_pool, get_string)
{
    mge::string_pool p;

    auto sv1 = p.get("wurst");
    auto sv2 = p.get(std::string("wurst"));

    EXPECT_EQ(sv1.begin(), sv2.begin());
    EXPECT_EQ(sv1.size(), sv2.size());
}

TEST(string_pool, get_string_view)
{
    mge::string_pool p;

    auto sv1 = p.get("wurst");
    auto sv2 = p.get("wurst"sv);

    EXPECT_EQ(sv1.begin(), sv2.begin());
    EXPECT_EQ(sv1.size(), sv2.size());
}

TEST(string_pool, size)
{
    mge::string_pool p;

    auto sv1 = p.get("wurst");
    auto sv2 = p.get("kaese");

    EXPECT_EQ(2u, p.size());
}
