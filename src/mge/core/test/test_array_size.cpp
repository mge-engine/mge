// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/array_size.hpp"
#include "test/googletest.hpp"

TEST(array_size, array_size)
{
    int x[5];
    EXPECT_EQ(5u, mge::array_size(x));
}
