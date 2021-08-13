// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/semantic_version.hpp"
#include "mge/core/stdexceptions.hpp"
#include "test/googletest.hpp"
#include "test/streamtest.hpp"
#include <iostream>
#include <string_view>

using namespace std::literals;

TEST(semantic_version, create)
{
    mge::semantic_version v(1u, 2u, 3u);
    EXPECT_STREAMOUTPUT("1.2.3", v);
}

TEST(semantic_version, empty)
{
    mge::semantic_version v;
    EXPECT_TRUE(v.empty());
}

TEST(semantic_version, from_string)
{
    mge::semantic_version v1("1.2.3"sv);
    EXPECT_EQ(1u, v1.major());
    EXPECT_EQ(2u, v1.minor());
    EXPECT_EQ(3u, v1.patch());

    mge::semantic_version v2("34"sv);
    EXPECT_EQ(34u, v2.major());

    try {
        mge::semantic_version v("34abc"sv);
        FAIL() << "Expected error";
    } catch (const mge::illegal_argument&) {
    }
}