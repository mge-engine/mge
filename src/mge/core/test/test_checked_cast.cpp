// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
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

// will not compile because it will throw always
// note it will work for values that pass the check
#if 0
TEST(checked_cast, constexpr_test)
{
    constexpr uint8_t val = mge::checked_cast<uint8_t>(1024);
}
#endif