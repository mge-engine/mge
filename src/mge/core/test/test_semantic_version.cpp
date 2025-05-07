// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
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

TEST(semantic_version, format)
{
    mge::semantic_version v1(1u, 2u, 3u);
    std::stringstream     ss1;
    ss1 << fmt::format("{}", v1);
    EXPECT_EQ("1.2.3", ss1.str());

    mge::semantic_version v2(1u, 2u, 0u);
    std::stringstream     ss2;
    ss2 << fmt::format("{}", v2);
    EXPECT_EQ("1.2", ss2.str());

    mge::semantic_version v3(1u, 0u, 0u);
    std::stringstream     ss3;
    ss3 << fmt::format("{}", v3);
    EXPECT_EQ("1", ss3.str());
}
