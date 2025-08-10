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
        EXPECT_EQ(type_void.name(), "void");
    }

    TEST(type, bool_type)
    {
        auto type_bool = type<bool>();
        EXPECT_FALSE(type_bool.is_void());
        EXPECT_TRUE(type_bool.is_bool());
        EXPECT_FALSE(type_bool.is_integral());
        EXPECT_EQ(type_bool.size(), sizeof(bool));
        EXPECT_EQ(type_bool.name(), "bool");
    }

    /*
        TEST(type, integral_type)
        {
            auto type_int = type<int>();
            EXPECT_FALSE(type_int.is_void());
            EXPECT_FALSE(type_int.is_bool());
            EXPECT_TRUE(type_int.is_integral());
            EXPECT_EQ(type_int.size(), sizeof(int));
        }
    */

    TEST(type, float_type)
    {
        auto type_float = type<float>();
        EXPECT_FALSE(type_float.is_void());
        EXPECT_FALSE(type_float.is_bool());
        EXPECT_FALSE(type_float.is_integral());
        EXPECT_TRUE(type_float.is_floating_point());
        EXPECT_EQ(type_float.size(), sizeof(float));
        EXPECT_EQ(type_float.name(), "float");
    }

    TEST(type, double_type)
    {
        auto type_double = type<double>();
        EXPECT_FALSE(type_double.is_void());
        EXPECT_FALSE(type_double.is_bool());
        EXPECT_FALSE(type_double.is_integral());
        EXPECT_TRUE(type_double.is_floating_point());
        EXPECT_EQ(type_double.size(), sizeof(double));
        EXPECT_EQ(type_double.name(), "double");
    }

} // namespace mge::reflection
