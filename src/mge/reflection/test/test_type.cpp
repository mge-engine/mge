// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/type.hpp"
#include "mge/reflection/type_details.hpp"
#include "mock_call_context.hpp"
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

    class non_copyable
    {
    public:
        non_copyable() = default;
        non_copyable(const non_copyable&) = delete;
        non_copyable& operator=(const non_copyable&) = delete;
        non_copyable(non_copyable&&) = default;
        non_copyable& operator=(non_copyable&&) = default;
    };

    class non_movable
    {
    public:
        non_movable() = default;
        non_movable(const non_movable&) = default;
        non_movable& operator=(const non_movable&) = default;
        non_movable(non_movable&&) = delete;
        non_movable& operator=(non_movable&&) = delete;
    };

    class copy_throws
    {
    public:
        copy_throws() = default;
        copy_throws(const copy_throws&) noexcept(false) {}
        copy_throws& operator=(const copy_throws&) noexcept(false)
        {
            return *this;
        }
    };

    class move_throws
    {
    public:
        move_throws() = default;
        move_throws(move_throws&&) noexcept(false) {}
        move_throws& operator=(move_throws&&) noexcept(false)
        {
            return *this;
        }
    };

    class abstract_class
    {
    public:
        virtual ~abstract_class() = default;
        virtual void pure_virtual_method() = 0;
    };

    class polymorphic_class
    {
    public:
        virtual ~polymorphic_class() {}
    };

    class non_polymorphic_class
    {
    public:
        ~non_polymorphic_class() {}
    };

    class non_destructible_class
    {
    public:
        ~non_destructible_class() = delete;
    };

    class non_empty_class
    {
    public:
        int value;
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
        EXPECT_EQ(type_a.alignment_of(), alignof(a));
        EXPECT_EQ(type<a>::name(), "mge::reflection::a");
        const auto& details = type_a.details();
        EXPECT_EQ(details->size, sizeof(a));
        EXPECT_EQ(details->alignment_of, alignof(a));
        EXPECT_TRUE(details->class_specific().is_constructible);
        EXPECT_TRUE(details->class_specific().is_default_constructible);
        EXPECT_TRUE(details->class_specific().is_default_constructor_noexcept);
        EXPECT_TRUE(details->class_specific().is_copy_constructible);
        EXPECT_TRUE(details->class_specific().is_copy_constructor_noexcept);
        EXPECT_TRUE(details->class_specific().is_move_constructible);
        EXPECT_TRUE(details->class_specific().is_move_constructor_noexcept);
        EXPECT_TRUE(details->class_specific().is_copy_assignable);
        EXPECT_TRUE(details->class_specific().is_copy_assignment_noexcept);
        EXPECT_TRUE(details->class_specific().is_move_assignable);
        EXPECT_TRUE(details->class_specific().is_move_assignment_noexcept);
        EXPECT_FALSE(details->class_specific().is_abstract);
        EXPECT_FALSE(details->class_specific().has_virtual_destructor);
        EXPECT_TRUE(details->class_specific().is_destructible);
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

    TEST(type, class_type_non_copyable)
    {
        auto type_non_copyable = type<non_copyable>();
        EXPECT_TRUE(type_non_copyable.is_class());
        const auto& details = type_non_copyable.details();
        EXPECT_FALSE(details->class_specific().is_copy_constructible);
        EXPECT_FALSE(details->class_specific().is_copy_constructor_noexcept);
        EXPECT_FALSE(details->class_specific().is_copy_assignable);
        EXPECT_FALSE(details->class_specific().is_copy_assignment_noexcept);
        EXPECT_TRUE(details->class_specific().is_move_constructible);
        EXPECT_TRUE(details->class_specific().is_move_constructor_noexcept);
        EXPECT_TRUE(details->class_specific().is_move_assignable);
        EXPECT_TRUE(details->class_specific().is_move_assignment_noexcept);
    }

    TEST(type, class_type_non_movable)
    {
        auto type_non_movable = type<non_movable>();
        EXPECT_TRUE(type_non_movable.is_class());
        const auto& details = type_non_movable.details();
        EXPECT_TRUE(details->class_specific().is_copy_constructible);
        EXPECT_TRUE(details->class_specific().is_copy_constructor_noexcept);
        EXPECT_TRUE(details->class_specific().is_copy_assignable);
        EXPECT_TRUE(details->class_specific().is_copy_assignment_noexcept);
        EXPECT_FALSE(details->class_specific().is_move_constructible);
        EXPECT_FALSE(details->class_specific().is_move_constructor_noexcept);
        EXPECT_FALSE(details->class_specific().is_move_assignable);
        EXPECT_FALSE(details->class_specific().is_move_assignment_noexcept);
    }

    TEST(type, class_type_copy_throws)
    {
        auto type_copy_throws = type<copy_throws>();
        EXPECT_TRUE(type_copy_throws.is_class());
        const auto& details = type_copy_throws.details();
        EXPECT_TRUE(details->class_specific().is_copy_constructible);
        EXPECT_FALSE(details->class_specific().is_copy_constructor_noexcept);
        EXPECT_TRUE(details->class_specific().is_copy_assignable);
        EXPECT_FALSE(details->class_specific().is_copy_assignment_noexcept);
    }

    TEST(type, class_type_move_throws)
    {
        auto type_move_throws = type<move_throws>();
        EXPECT_TRUE(type_move_throws.is_class());
        const auto& details = type_move_throws.details();
        EXPECT_TRUE(details->class_specific().is_move_constructible);
        EXPECT_FALSE(details->class_specific().is_move_constructor_noexcept);
        EXPECT_TRUE(details->class_specific().is_move_assignable);
        EXPECT_FALSE(details->class_specific().is_move_assignment_noexcept);
    }

    TEST(type, class_type_abstract)
    {
        auto type_abstract_class = type<abstract_class>();
        EXPECT_TRUE(type_abstract_class.is_class());
        const auto& details = type_abstract_class.details();
        EXPECT_TRUE(details->class_specific().is_abstract);
        EXPECT_FALSE(details->class_specific().is_constructible);
        EXPECT_FALSE(details->class_specific().is_default_constructible);
        EXPECT_TRUE(details->class_specific().has_virtual_destructor);
    }

    TEST(type, class_type_polymorphic)
    {
        auto type_polymorphic_class = type<polymorphic_class>();
        EXPECT_TRUE(type_polymorphic_class.is_class());
        const auto& details = type_polymorphic_class.details();
        EXPECT_FALSE(details->class_specific().is_abstract);
        EXPECT_TRUE(details->class_specific().has_virtual_destructor);
    }

    TEST(type, class_type_non_polymorphic)
    {
        auto type_non_polymorphic_class = type<non_polymorphic_class>();
        EXPECT_TRUE(type_non_polymorphic_class.is_class());
        const auto& details = type_non_polymorphic_class.details();
        EXPECT_FALSE(details->class_specific().is_abstract);
        EXPECT_FALSE(details->class_specific().has_virtual_destructor);
        EXPECT_TRUE(details->class_specific().is_destructible);
    }

    TEST(type, class_type_non_destructible)
    {
        auto type_non_destructible_class = type<non_destructible_class>();
        EXPECT_TRUE(type_non_destructible_class.is_class());
        const auto& details = type_non_destructible_class.details();
        EXPECT_FALSE(details->class_specific().is_destructible);
    }

    TEST(type, class_type_non_empty)
    {
        auto type_non_empty = type<non_empty_class>();
        EXPECT_TRUE(type_non_empty.is_class());
        const auto& details = type_non_empty.details();
        EXPECT_FALSE(details->class_specific().is_empty);
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

    struct test_default_constructor
    {
        int  value = 42;
        bool constructed = false;

        test_default_constructor()
        {
            constructed = true;
        }
    };

    TEST(type, default_constructor_in_constructors_vector)
    {
        // Constructor is automatically registered during type creation
        const auto& details = type<test_default_constructor>().details();
        const auto& class_details = details->class_specific();

        ASSERT_EQ(class_details.constructors.size(), 1);

        const auto& [sig, invoke_fn] = class_details.constructors[0];
        EXPECT_EQ(sig.return_type(), make_type_identifier<void>());
        EXPECT_TRUE(sig.parameter_types().empty());

        alignas(test_default_constructor) char
                          buffer[sizeof(test_default_constructor)];
        MOCK_call_context ctx;
        EXPECT_CALL(ctx, this_ptr()).WillOnce(testing::Return(buffer));

        invoke_fn(ctx);

        auto* obj = reinterpret_cast<test_default_constructor*>(buffer);
        EXPECT_EQ(obj->value, 42);
        EXPECT_TRUE(obj->constructed);

        obj->~test_default_constructor();
    }

    struct test_destructor
    {
        static inline int destruction_count = 0;
        int               value = 99;

        ~test_destructor()
        {
            ++destruction_count;
        }
    };

    TEST(type, destructor_invocation)
    {
        test_destructor::destruction_count = 0;

        const auto& details = type<test_destructor>().details();
        const auto& class_details = details->class_specific();

        ASSERT_TRUE(class_details.destructor);

        alignas(test_destructor) char buffer[sizeof(test_destructor)];
        auto*                         obj = new (buffer) test_destructor();
        EXPECT_EQ(obj->value, 99);
        EXPECT_EQ(test_destructor::destruction_count, 0);

        MOCK_call_context ctx;
        EXPECT_CALL(ctx, this_ptr()).WillOnce(testing::Return(buffer));

        class_details.destructor(ctx);

        EXPECT_EQ(test_destructor::destruction_count, 1);
    }

    struct throwing_constructor
    {
        throwing_constructor()
        {
            throw std::runtime_error("Constructor failed");
        }
    };

    TEST(type, constructor_exception_handling)
    {
        const auto& details = type<throwing_constructor>().details();
        const auto& class_details = details->class_specific();

        ASSERT_EQ(class_details.constructors.size(), 1);

        const auto& [sig, invoke_fn] = class_details.constructors[0];

        alignas(throwing_constructor) char buffer[sizeof(throwing_constructor)];
        MOCK_call_context                  ctx;
        EXPECT_CALL(ctx, this_ptr()).WillOnce(testing::Return(buffer));
        EXPECT_CALL(ctx, exception_thrown(testing::A<const std::exception&>()))
            .Times(1);

        invoke_fn(ctx);
    }

    struct throwing_destructor
    {
        bool should_throw = true;

        ~throwing_destructor() noexcept(false)
        {
            if (should_throw) {
                throw std::runtime_error("Destructor failed");
            }
        }
    };

    TEST(type, destructor_exception_handling)
    {
        const auto& details = type<throwing_destructor>().details();
        const auto& class_details = details->class_specific();

        ASSERT_TRUE(class_details.destructor);

        alignas(throwing_destructor) char buffer[sizeof(throwing_destructor)];
        auto* obj = new (buffer) throwing_destructor();
        obj->should_throw = true;

        MOCK_call_context ctx;
        EXPECT_CALL(ctx, this_ptr()).WillOnce(testing::Return(buffer));
        EXPECT_CALL(ctx, exception_thrown(testing::A<const std::exception&>()))
            .Times(1);

        class_details.destructor(ctx);
    }

    struct test_parameterized_constructor
    {
        int   a = 0;
        float b = 0.0f;

        test_parameterized_constructor() = default;

        test_parameterized_constructor(int x, float y)
            : a(x)
            , b(y)
        {}
    };

    TEST(type, parameterized_constructor_registration)
    {
        auto type_test =
            type<test_parameterized_constructor>().constructor<int, float>();

        const auto& details = type_test.details();
        const auto& class_details = details->class_specific();

        // Should have 2 constructors: default (auto-registered) + parameterized
        ASSERT_EQ(class_details.constructors.size(), 2);

        // Find the parameterized constructor
        const signature expected_sig(
            make_type_identifier<void>(),
            {make_type_identifier<int>(), make_type_identifier<float>()});

        bool found = false;
        for (const auto& [sig, invoke_fn] : class_details.constructors) {
            if (sig == expected_sig) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST(type, duplicate_constructor_registration_ignored)
    {
        auto type_test = type<test_parameterized_constructor>()
                             .constructor<int, float>()
                             .constructor<int, float>();

        const auto& details = type_test.details();
        const auto& class_details = details->class_specific();

        // Should still have only 2 constructors (not 3)
        EXPECT_EQ(class_details.constructors.size(), 2);
    }

    TEST(type, parameterized_constructor_invocation)
    {
        auto type_test =
            type<test_parameterized_constructor>().constructor<int, float>();

        const auto& details = type_test.details();
        const auto& class_details = details->class_specific();

        // Find the parameterized constructor
        const signature expected_sig(
            make_type_identifier<void>(),
            {make_type_identifier<int>(), make_type_identifier<float>()});

        invoke_function_type invoke_fn;
        bool                 found = false;
        for (const auto& [sig, fn] : class_details.constructors) {
            if (sig == expected_sig) {
                invoke_fn = fn;
                found = true;
                break;
            }
        }
        ASSERT_TRUE(found);

        // Create buffer and invoke constructor
        alignas(test_parameterized_constructor) char
                          buffer[sizeof(test_parameterized_constructor)];
        MOCK_call_context ctx;
        EXPECT_CALL(ctx, this_ptr()).WillOnce(testing::Return(buffer));
        EXPECT_CALL(ctx, int32_t_parameter(0)).WillOnce(testing::Return(123));
        EXPECT_CALL(ctx, float_parameter(1)).WillOnce(testing::Return(45.6f));

        invoke_fn(ctx);

        auto* obj = reinterpret_cast<test_parameterized_constructor*>(buffer);
        EXPECT_EQ(obj->a, 123);
        EXPECT_FLOAT_EQ(obj->b, 45.6f);

        obj->~test_parameterized_constructor();
    }

    struct test_reference_constructor
    {
        int value;

        test_reference_constructor(const int& ref)
            : value(ref)
        {}
    };

    TEST(type, reference_parameter_constructor_throws_not_implemented)
    {
        auto type_test =
            type<test_reference_constructor>().constructor<const int&>();

        const auto& details = type_test.details();
        const auto& class_details = details->class_specific();

        // Find the reference constructor
        const signature expected_sig(make_type_identifier<void>(),
                                     {make_type_identifier<const int&>()});

        invoke_function_type invoke_fn;
        bool                 found = false;
        for (const auto& [sig, fn] : class_details.constructors) {
            if (sig == expected_sig) {
                invoke_fn = fn;
                found = true;
                break;
            }
        }
        ASSERT_TRUE(found);

        // Create buffer and try to invoke constructor
        alignas(test_reference_constructor) char
                          buffer[sizeof(test_reference_constructor)];
        MOCK_call_context ctx;
        EXPECT_CALL(ctx, this_ptr()).WillOnce(testing::Return(buffer));
        EXPECT_CALL(ctx, exception_thrown(testing::A<const std::exception&>()))
            .Times(1);

        invoke_fn(ctx);
    }

} // namespace mge::reflection
