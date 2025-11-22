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
        EXPECT_FALSE(type_void.is_floating_point());
        EXPECT_FALSE(type_void.is_enum());
        EXPECT_FALSE(type_void.is_class());
        EXPECT_FALSE(type_void.is_pointer());
        EXPECT_FALSE(type_void.is_array());
        EXPECT_FALSE(type_void.is_reference());
        EXPECT_EQ(type_void.size(), 0);
        EXPECT_EQ(type<void>::name(), "void");
    }

    TEST(type, bool_type)
    {
        auto type_bool = type<bool>();
        EXPECT_FALSE(type_bool.is_void());
        EXPECT_TRUE(type_bool.is_bool());
        EXPECT_FALSE(type_bool.is_integral());
        EXPECT_FALSE(type_bool.is_floating_point());
        EXPECT_FALSE(type_bool.is_enum());
        EXPECT_FALSE(type_bool.is_class());
        EXPECT_FALSE(type_bool.is_pointer());
        EXPECT_FALSE(type_bool.is_array());
        EXPECT_FALSE(type_bool.is_reference());
        EXPECT_EQ(type_bool.size(), sizeof(bool));
        EXPECT_EQ(type<bool>::name(), "bool");
    }

    TEST(type, char_type)
    {
        auto type_char = type<char>();
        EXPECT_FALSE(type_char.is_void());
        EXPECT_FALSE(type_char.is_bool());
        EXPECT_TRUE(type_char.is_integral());
        EXPECT_FALSE(type_char.is_enum());
        EXPECT_EQ(type_char.size(), sizeof(char));
        EXPECT_EQ(type<char>::name(), "char");
    }
    TEST(type, unsigned_char_type)
    {
        auto type_unsigned_char = type<unsigned char>();
        EXPECT_FALSE(type_unsigned_char.is_void());
        EXPECT_FALSE(type_unsigned_char.is_bool());
        EXPECT_TRUE(type_unsigned_char.is_integral());
        EXPECT_FALSE(type_unsigned_char.is_enum());
        EXPECT_EQ(type_unsigned_char.size(), sizeof(unsigned char));
        EXPECT_EQ(type<unsigned char>::name(), "unsigned char");
    }

    TEST(type, signed_char_type)
    {
        auto type_signed_char = type<signed char>();
        EXPECT_FALSE(type_signed_char.is_void());
        EXPECT_FALSE(type_signed_char.is_bool());
        EXPECT_TRUE(type_signed_char.is_integral());
        EXPECT_FALSE(type_signed_char.is_enum());
        EXPECT_EQ(type_signed_char.size(), sizeof(signed char));
        EXPECT_EQ(type<signed char>::name(), "signed char");
    }
    TEST(type, short_type)
    {
        auto type_short = type<short>();
        EXPECT_FALSE(type_short.is_void());
        EXPECT_FALSE(type_short.is_bool());
        EXPECT_TRUE(type_short.is_integral());
        EXPECT_FALSE(type_short.is_enum());
        EXPECT_EQ(type_short.size(), sizeof(short));
        EXPECT_EQ(type<short>::name(), "short");
    }

    TEST(type, unsigned_short_type)
    {
        auto type_unsigned_short = type<unsigned short>();
        EXPECT_FALSE(type_unsigned_short.is_void());
        EXPECT_FALSE(type_unsigned_short.is_bool());
        EXPECT_TRUE(type_unsigned_short.is_integral());
        EXPECT_FALSE(type_unsigned_short.is_enum());
        EXPECT_EQ(type_unsigned_short.size(), sizeof(unsigned short));
        EXPECT_EQ(type<unsigned short>::name(), "unsigned short");
    }

    TEST(type, int_type)
    {
        auto type_int = type<int>();
        EXPECT_FALSE(type_int.is_void());
        EXPECT_FALSE(type_int.is_bool());
        EXPECT_TRUE(type_int.is_integral());
        EXPECT_FALSE(type_int.is_enum());
        EXPECT_EQ(type_int.size(), sizeof(int));
        EXPECT_EQ(type<int>::name(), "int");
    }

    TEST(type, unsigned_int_type)
    {
        auto type_unsigned_int = type<unsigned int>();
        EXPECT_FALSE(type_unsigned_int.is_void());
        EXPECT_FALSE(type_unsigned_int.is_bool());
        EXPECT_TRUE(type_unsigned_int.is_integral());
        EXPECT_FALSE(type_unsigned_int.is_enum());
        EXPECT_EQ(type_unsigned_int.size(), sizeof(unsigned int));
        EXPECT_EQ(type<unsigned int>::name(), "unsigned int");
    }
    TEST(type, long_type)
    {
        auto type_long = type<long>();
        EXPECT_FALSE(type_long.is_void());
        EXPECT_FALSE(type_long.is_bool());
        EXPECT_TRUE(type_long.is_integral());
        EXPECT_FALSE(type_long.is_enum());
        EXPECT_EQ(type_long.size(), sizeof(long));
        EXPECT_EQ(type<long>::name(), "long");
    }

    TEST(type, unsigned_long_type)
    {
        auto type_unsigned_long = type<unsigned long>();
        EXPECT_FALSE(type_unsigned_long.is_void());
        EXPECT_FALSE(type_unsigned_long.is_bool());
        EXPECT_TRUE(type_unsigned_long.is_integral());
        EXPECT_FALSE(type_unsigned_long.is_enum());
        EXPECT_EQ(type_unsigned_long.size(), sizeof(unsigned long));
        EXPECT_EQ(type<unsigned long>::name(), "unsigned long");
    }

    TEST(type, long_long_type)
    {
        auto type_long_long = type<long long>();
        EXPECT_FALSE(type_long_long.is_void());
        EXPECT_FALSE(type_long_long.is_bool());
        EXPECT_TRUE(type_long_long.is_integral());
        EXPECT_FALSE(type_long_long.is_enum());
        EXPECT_EQ(type_long_long.size(), sizeof(long long));
        EXPECT_EQ(type<long long>::name(), "int64_t");
    }

    TEST(type, unsigned_long_long_type)
    {
        auto type_unsigned_long_long = type<unsigned long long>();
        EXPECT_FALSE(type_unsigned_long_long.is_void());
        EXPECT_FALSE(type_unsigned_long_long.is_bool());
        EXPECT_TRUE(type_unsigned_long_long.is_integral());
        EXPECT_FALSE(type_unsigned_long_long.is_enum());
        EXPECT_EQ(type_unsigned_long_long.size(), sizeof(unsigned long long));
        EXPECT_EQ(type<unsigned long long>::name(), "uint64_t");
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
        EXPECT_EQ(type<float>::name(), "float");
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
        EXPECT_EQ(type<double>::name(), "double");
    }

    TEST(type, long_double_type)
    {
        auto type_long_double = type<long double>();
        EXPECT_FALSE(type_long_double.is_void());
        EXPECT_FALSE(type_long_double.is_bool());
        EXPECT_FALSE(type_long_double.is_integral());
        EXPECT_TRUE(type_long_double.is_floating_point());
        EXPECT_FALSE(type_long_double.is_enum());
        EXPECT_FALSE(type_long_double.is_class());
        EXPECT_FALSE(type_long_double.is_pointer());
        EXPECT_FALSE(type_long_double.is_array());
        EXPECT_FALSE(type_long_double.is_reference());
        EXPECT_EQ(type_long_double.size(), sizeof(long double));
        EXPECT_EQ(type<long double>::name(), "long double");
    }

    enum class enum1
    {
        RED,
        GREEN,
        BLUE
    };

    enum class uenum : uint64_t
    {
        ALPHA = 10,
        BETA = 20,
        GAMMA = 30
    };

    enum class duplicate_enum : int
    {
        VALUE1 = 1,
        VALUE2 = 2,
        VALUE1_DUPLICATE = 1
    };

    TEST(type, enum_type_signed)
    {
        auto type_enum1 = type<enum1>();
        EXPECT_FALSE(type_enum1.is_void());
        EXPECT_FALSE(type_enum1.is_bool());
        EXPECT_FALSE(type_enum1.is_integral());
        EXPECT_FALSE(type_enum1.is_floating_point());
        EXPECT_TRUE(type_enum1.is_enum());
        EXPECT_EQ(type_enum1.size(), sizeof(enum1));
        EXPECT_EQ(type<enum1>::name(), "enum1");

        const auto& details = type_enum1.details();
        EXPECT_TRUE(details->is_enum);
        EXPECT_EQ(details->enum_specific().underlying_type,
                  get_or_create_type_details<int>());
        EXPECT_EQ(details->enum_specific().values.size(), 3);
        EXPECT_EQ(details->enum_specific().values[0].first, "RED");
        EXPECT_EQ(std::get<int64_t>(details->enum_specific().values[0].second),
                  0);
        EXPECT_EQ(details->enum_specific().values[1].first, "GREEN");
        EXPECT_EQ(std::get<int64_t>(details->enum_specific().values[1].second),
                  1);
        EXPECT_EQ(details->enum_specific().values[2].first, "BLUE");
        EXPECT_EQ(std::get<int64_t>(details->enum_specific().values[2].second),
                  2);
    }

    TEST(type, enum_type_unsigned)
    {
        auto type_uenum = type<uenum>();
        EXPECT_FALSE(type_uenum.is_void());
        EXPECT_FALSE(type_uenum.is_bool());
        EXPECT_FALSE(type_uenum.is_integral());
        EXPECT_FALSE(type_uenum.is_floating_point());
        EXPECT_TRUE(type_uenum.is_enum());
        const auto& udetails = type_uenum.details();
        EXPECT_TRUE(udetails->is_enum);
        EXPECT_EQ(udetails->enum_specific().underlying_type,
                  get_or_create_type_details<uint64_t>());
        EXPECT_EQ(udetails->enum_specific().values.size(), 3);
        EXPECT_EQ(udetails->enum_specific().values[0].first, "ALPHA");
        EXPECT_EQ(
            std::get<uint64_t>(udetails->enum_specific().values[0].second),
            10);
        EXPECT_EQ(udetails->enum_specific().values[1].first, "BETA");
        EXPECT_EQ(
            std::get<uint64_t>(udetails->enum_specific().values[1].second),
            20);
        EXPECT_EQ(udetails->enum_specific().values[2].first, "GAMMA");
        EXPECT_EQ(
            std::get<uint64_t>(udetails->enum_specific().values[2].second),
            30);
    }

    TEST(type, enum_type_duplicate_values)
    {
        auto type_duplicate_enum =
            type<duplicate_enum>().value("VALUE1_DUPLICATE",
                                         duplicate_enum::VALUE1_DUPLICATE);
        EXPECT_FALSE(type_duplicate_enum.is_void());
        EXPECT_FALSE(type_duplicate_enum.is_bool());
        EXPECT_FALSE(type_duplicate_enum.is_integral());
        EXPECT_FALSE(type_duplicate_enum.is_floating_point());
        EXPECT_TRUE(type_duplicate_enum.is_enum());
        const auto& ddetails = type_duplicate_enum.details();
        EXPECT_TRUE(ddetails->is_enum);
        EXPECT_EQ(ddetails->enum_specific().underlying_type,
                  get_or_create_type_details<int>());
        ASSERT_EQ(ddetails->enum_specific().values.size(), 3);
        EXPECT_EQ(ddetails->enum_specific().values[0].first, "VALUE1");
        EXPECT_EQ(std::get<int64_t>(ddetails->enum_specific().values[0].second),
                  1);
        EXPECT_EQ(ddetails->enum_specific().values[1].first, "VALUE2");
        EXPECT_EQ(std::get<int64_t>(ddetails->enum_specific().values[1].second),
                  2);
        EXPECT_EQ(ddetails->enum_specific().values[2].first,
                  "VALUE1_DUPLICATE");
        EXPECT_EQ(std::get<int64_t>(ddetails->enum_specific().values[2].second),
                  1);
    }

    class a
    {};

    class non_default_constructible
    {
    public:
        non_default_constructible(int x)
            : value(x)
        {}
        int value;
    };

    class default_constructible_throws
    {
    public:
        default_constructible_throws() noexcept(false) {}
    };

    class default_constructible_noexcept
    {
    public:
        default_constructible_noexcept() noexcept {}
    };

    TEST(type, class_type)
    {
        auto type_a = type<a>();
        EXPECT_FALSE(type_a.is_void());
        EXPECT_FALSE(type_a.is_bool());
        EXPECT_FALSE(type_a.is_integral());
        EXPECT_FALSE(type_a.is_floating_point());
        EXPECT_FALSE(type_a.is_enum());
        EXPECT_TRUE(type_a.is_class());
        EXPECT_EQ(type_a.size(), sizeof(a));
        EXPECT_EQ(type<a>::name(), "mge::reflection::a");
        const auto& details = type_a.details();
        EXPECT_TRUE(details->class_specific().is_default_constructible);
        EXPECT_TRUE(details->class_specific().is_default_constructor_noexcept);
    }

    TEST(type, class_type_not_default_constructible)
    {
        auto type_non_default_constructible = type<non_default_constructible>();
        EXPECT_TRUE(type_non_default_constructible.is_class());
        const auto& details = type_non_default_constructible.details();
        EXPECT_FALSE(details->class_specific().is_default_constructible);
        EXPECT_FALSE(details->class_specific().is_default_constructor_noexcept);
    }

    TEST(type, class_type_default_constructor_throws)
    {
        auto type_default_constructible_throws =
            type<default_constructible_throws>();
        EXPECT_TRUE(type_default_constructible_throws.is_class());
        const auto& details = type_default_constructible_throws.details();
        EXPECT_TRUE(details->class_specific().is_default_constructible);
        EXPECT_FALSE(details->class_specific().is_default_constructor_noexcept);
    }

    TEST(type, class_type_default_constructor_noexcept)
    {
        auto type_default_constructible_noexcept =
            type<default_constructible_noexcept>();
        EXPECT_TRUE(type_default_constructible_noexcept.is_class());
        const auto& details = type_default_constructible_noexcept.details();
        EXPECT_TRUE(details->class_specific().is_default_constructible);
        EXPECT_TRUE(details->class_specific().is_default_constructor_noexcept);
    }

    class derived : public a
    {};

    TEST(type, class_type_with_base)
    {
        auto type_derived = type<derived>().base<a>();
        EXPECT_FALSE(type_derived.is_void());
        EXPECT_FALSE(type_derived.is_bool());
        EXPECT_FALSE(type_derived.is_integral());
        EXPECT_FALSE(type_derived.is_floating_point());
        EXPECT_FALSE(type_derived.is_enum());
        EXPECT_TRUE(type_derived.is_class());
        EXPECT_EQ(type_derived.size(), sizeof(derived));
        EXPECT_EQ(type<derived>::name(), "mge::reflection::derived");

        const auto& details = type_derived.details();
        EXPECT_TRUE(details->is_class);
        const auto& class_details = details->class_specific();
        ASSERT_EQ(class_details.bases.size(), 1);
        EXPECT_EQ(class_details.bases[0], get_or_create_type_details<a>());
    }

    TEST(type, pointer_type)
    {
        auto type_int_ptr = type<int*>();
        EXPECT_FALSE(type_int_ptr.is_void());
        EXPECT_FALSE(type_int_ptr.is_bool());
        EXPECT_FALSE(type_int_ptr.is_integral());
        EXPECT_FALSE(type_int_ptr.is_floating_point());
        EXPECT_FALSE(type_int_ptr.is_enum());
        EXPECT_FALSE(type_int_ptr.is_class());
        EXPECT_TRUE(type_int_ptr.is_pointer());
        EXPECT_EQ(type_int_ptr.size(), sizeof(int*));
        EXPECT_EQ(type<int*>::name(), "int*");

        const auto& details = type_int_ptr.details();
        EXPECT_TRUE(details->is_pointer);
        EXPECT_EQ(details->pointer_specific().element_type,
                  get_or_create_type_details<int>());
    }

    TEST(type, array_type)
    {
        auto type_int_array = type<int[10]>();
        EXPECT_FALSE(type_int_array.is_void());
        EXPECT_FALSE(type_int_array.is_bool());
        EXPECT_FALSE(type_int_array.is_integral());
        EXPECT_FALSE(type_int_array.is_floating_point());
        EXPECT_FALSE(type_int_array.is_enum());
        EXPECT_FALSE(type_int_array.is_class());
        EXPECT_FALSE(type_int_array.is_pointer());
        EXPECT_TRUE(type_int_array.is_array());
        EXPECT_EQ(type_int_array.size(), sizeof(int[10]));
        EXPECT_EQ(type<int[10]>::name(), "int[10]");

        const auto& details = type_int_array.details();
        EXPECT_TRUE(details->is_array);
        const auto& array_details = details->array_specific();
        EXPECT_EQ(array_details.element_type,
                  get_or_create_type_details<int>());
        EXPECT_EQ(array_details.size, 10);
    }

    TEST(type, reference_type)
    {
        static_assert(std::is_reference_v<int&>);
        static_assert(!std::is_reference_v<int>);

        auto type_int_ref = type<int&>();
        EXPECT_FALSE(type_int_ref.is_void());
        EXPECT_FALSE(type_int_ref.is_bool());
        EXPECT_FALSE(type_int_ref.is_integral());
        EXPECT_FALSE(type_int_ref.is_floating_point());
        EXPECT_FALSE(type_int_ref.is_enum());
        EXPECT_FALSE(type_int_ref.is_class());
        EXPECT_FALSE(type_int_ref.is_pointer());
        EXPECT_FALSE(type_int_ref.is_array());
        EXPECT_TRUE(type_int_ref.is_reference());
        EXPECT_EQ(type_int_ref.size(), sizeof(int&));
        EXPECT_EQ(type<int&>::name(), "int&");

        const auto& details = type_int_ref.details();
        EXPECT_TRUE(details->is_reference);
        const auto& reference_details = details->reference_specific();
        EXPECT_EQ(reference_details.referenced_type,
                  get_or_create_type_details<int>());
    }

} // namespace mge::reflection
