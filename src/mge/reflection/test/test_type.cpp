// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/type.hpp"
#include "mge/reflection/type_details.hpp"
#include "test/googletest.hpp"

namespace mge::reflection {

    TEST(type, is_void)
    {
        auto type_void = type<void>();
        EXPECT_TRUE(type_void.is_void());
        EXPECT_FALSE(type_void.is_integral());
        EXPECT_EQ(type_void.size(), 0);
    }
} // namespace mge::reflection
