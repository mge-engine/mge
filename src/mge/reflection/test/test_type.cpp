// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/type.hpp"
#include "mge/reflection/type_details.hpp"
#include "test/googletest.hpp"

namespace mge::reflection {

    TEST(type, void_type)
    {
        auto type_void = type<void>();
        EXPECT_TRUE(type_void.is_void());
        EXPECT_FALSE(type_void.is_bool());
        EXPECT_FALSE(type_void.is_integral());
        EXPECT_EQ(type_void.size(), 0);
    }

    TEST(type, bool_type)
    {
        auto type_bool = type<bool>();
        EXPECT_FALSE(type_bool.is_void());
        EXPECT_TRUE(type_bool.is_bool());
        EXPECT_FALSE(type_bool.is_integral());
        EXPECT_EQ(type_bool.size(), sizeof(bool));
    }
} // namespace mge::reflection
