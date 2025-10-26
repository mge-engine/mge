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
        EXPECT_FALSE(type_void.is_enum());
        EXPECT_EQ(type_void.size(), 0);
        EXPECT_EQ(type_void.name(), "void");
    }

    TEST(type, bool_type)
    {
        auto type_bool = type<bool>();
        EXPECT_FALSE(type_bool.is_void());
        EXPECT_TRUE(type_bool.is_bool());
        EXPECT_FALSE(type_bool.is_integral());
        EXPECT_FALSE(type_bool.is_enum());
        EXPECT_EQ(type_bool.size(), sizeof(bool));
        EXPECT_EQ(type_bool.name(), "bool");
    }

    TEST(type, char_type)
    {
        auto type_char = type<char>();
        EXPECT_FALSE(type_char.is_void());
        EXPECT_FALSE(type_char.is_bool());
        EXPECT_TRUE(type_char.is_integral());
        EXPECT_FALSE(type_char.is_enum());
        EXPECT_EQ(type_char.size(), sizeof(char));
        EXPECT_EQ(type_char.name(), "char");
    }
    TEST(type, unsigned_char_type)
    {
        auto type_unsigned_char = type<unsigned char>();
        EXPECT_FALSE(type_unsigned_char.is_void());
        EXPECT_FALSE(type_unsigned_char.is_bool());
        EXPECT_TRUE(type_unsigned_char.is_integral());
        EXPECT_FALSE(type_unsigned_char.is_enum());
        EXPECT_EQ(type_unsigned_char.size(), sizeof(unsigned char));
        EXPECT_EQ(type_unsigned_char.name(), "unsigned char");
    }

    TEST(type, signed_char_type)
    {
        auto type_signed_char = type<signed char>();
        EXPECT_FALSE(type_signed_char.is_void());
        EXPECT_FALSE(type_signed_char.is_bool());
        EXPECT_TRUE(type_signed_char.is_integral());
        EXPECT_FALSE(type_signed_char.is_enum());
        EXPECT_EQ(type_signed_char.size(), sizeof(signed char));
        EXPECT_EQ(type_signed_char.name(), "signed char");
    }
    TEST(type, short_type)
    {
        auto type_short = type<short>();
        EXPECT_FALSE(type_short.is_void());
        EXPECT_FALSE(type_short.is_bool());
        EXPECT_TRUE(type_short.is_integral());
        EXPECT_FALSE(type_short.is_enum());
        EXPECT_EQ(type_short.size(), sizeof(short));
        EXPECT_EQ(type_short.name(), "short");
    }

    TEST(type, unsigned_short_type)
    {
        auto type_unsigned_short = type<unsigned short>();
        EXPECT_FALSE(type_unsigned_short.is_void());
        EXPECT_FALSE(type_unsigned_short.is_bool());
        EXPECT_TRUE(type_unsigned_short.is_integral());
        EXPECT_FALSE(type_unsigned_short.is_enum());
        EXPECT_EQ(type_unsigned_short.size(), sizeof(unsigned short));
        EXPECT_EQ(type_unsigned_short.name(), "unsigned short");
    }

    TEST(type, int_type)
    {
        auto type_int = type<int>();
        EXPECT_FALSE(type_int.is_void());
        EXPECT_FALSE(type_int.is_bool());
        EXPECT_TRUE(type_int.is_integral());
        EXPECT_FALSE(type_int.is_enum());
        EXPECT_EQ(type_int.size(), sizeof(int));
        EXPECT_EQ(type_int.name(), "int");
    }

    TEST(type, unsigned_int_type)
    {
        auto type_unsigned_int = type<unsigned int>();
        EXPECT_FALSE(type_unsigned_int.is_void());
        EXPECT_FALSE(type_unsigned_int.is_bool());
        EXPECT_TRUE(type_unsigned_int.is_integral());
        EXPECT_FALSE(type_unsigned_int.is_enum());
        EXPECT_EQ(type_unsigned_int.size(), sizeof(unsigned int));
        EXPECT_EQ(type_unsigned_int.name(), "unsigned int");
    }
    TEST(type, long_type)
    {
        auto type_long = type<long>();
        EXPECT_FALSE(type_long.is_void());
        EXPECT_FALSE(type_long.is_bool());
        EXPECT_TRUE(type_long.is_integral());
        EXPECT_FALSE(type_long.is_enum());
        EXPECT_EQ(type_long.size(), sizeof(long));
        EXPECT_EQ(type_long.name(), "long");
    }

    TEST(type, unsigned_long_type)
    {
        auto type_unsigned_long = type<unsigned long>();
        EXPECT_FALSE(type_unsigned_long.is_void());
        EXPECT_FALSE(type_unsigned_long.is_bool());
        EXPECT_TRUE(type_unsigned_long.is_integral());
        EXPECT_FALSE(type_unsigned_long.is_enum());
        EXPECT_EQ(type_unsigned_long.size(), sizeof(unsigned long));
        EXPECT_EQ(type_unsigned_long.name(), "unsigned long");
    }

    TEST(type, long_long_type)
    {
        auto type_long_long = type<long long>();
        EXPECT_FALSE(type_long_long.is_void());
        EXPECT_FALSE(type_long_long.is_bool());
        EXPECT_TRUE(type_long_long.is_integral());
        EXPECT_FALSE(type_long_long.is_enum());
        EXPECT_EQ(type_long_long.size(), sizeof(long long));
        EXPECT_EQ(type_long_long.name(), "int64_t");
    }

    TEST(type, unsigned_long_long_type)
    {
        auto type_unsigned_long_long = type<unsigned long long>();
        EXPECT_FALSE(type_unsigned_long_long.is_void());
        EXPECT_FALSE(type_unsigned_long_long.is_bool());
        EXPECT_TRUE(type_unsigned_long_long.is_integral());
        EXPECT_FALSE(type_unsigned_long_long.is_enum());
        EXPECT_EQ(type_unsigned_long_long.size(), sizeof(unsigned long long));
        EXPECT_EQ(type_unsigned_long_long.name(), "uint64_t");
    }
    TEST(type, float_type)
    {
        auto type_float = type<float>();
        EXPECT_FALSE(type_float.is_void());
        EXPECT_FALSE(type_float.is_bool());
        EXPECT_FALSE(type_float.is_integral());
        EXPECT_TRUE(type_float.is_floating_point());
        EXPECT_FALSE(type_float.is_enum());
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
        EXPECT_FALSE(type_double.is_enum());
        EXPECT_EQ(type_double.size(), sizeof(double));
        EXPECT_EQ(type_double.name(), "double");
    }

    enum class enum1
    {
        RED,
        GREEN,
        BLUE
    };

    TEST(type, enum_type)
    {
        auto type_enum1 = type<enum1>();
        EXPECT_FALSE(type_enum1.is_void());
        EXPECT_FALSE(type_enum1.is_bool());
        EXPECT_FALSE(type_enum1.is_integral());
        EXPECT_FALSE(type_enum1.is_floating_point());
        EXPECT_TRUE(type_enum1.is_enum());
        EXPECT_EQ(type_enum1.size(), sizeof(enum1));
        EXPECT_EQ(type_enum1.name(), "enum1");
    }

} // namespace mge::reflection
