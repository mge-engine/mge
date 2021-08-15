// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/checked_cast.hpp"
#include "test/googletest.hpp"
#include <cstdint>

TEST(checked_cast, success)
{
    uint8_t x = mge::checked_cast<uint8_t>(123);
    EXPECT_EQ(123u, x);
}

TEST(checked_cast, failure)
{
    EXPECT_THROW(mge::checked_cast<uint8_t>(1024), mge::bad_cast);
}
