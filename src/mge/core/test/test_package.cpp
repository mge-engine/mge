// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/package.hpp"
#include "test/googletest.hpp"
#include <string_view>

TEST(package, mge)
{
    using namespace std::literals::string_view_literals;
    auto mge = mge::package::get("mge"sv);
    ASSERT_NE(mge, nullptr);
    EXPECT_EQ("mge", mge->name());
}