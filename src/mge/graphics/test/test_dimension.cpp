// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/dimension.hpp"
#include "test/googletest.hpp"

TEST(dimension, default_ctor)
{
    mge::dimension d;
    EXPECT_EQ(0u, d.width);
    EXPECT_EQ(0u, d.height);
}
