// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/semantic_version.hpp"
#include "test/googletest.hpp"
#include "test/streamtest.hpp"
#include <iostream>

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