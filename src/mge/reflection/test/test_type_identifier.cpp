// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/type_name.hpp"
#include "mge/reflection/type_identifier.hpp"
#include "test/googletest.hpp"
#include "test/test_format.hpp"
namespace mge::reflection {

    TEST(type_identifier, basic)
    {
        type_identifier id(typeid(int), mge::type_name<int>(), false, false, false);
        EXPECT_EQ(id.type_index(), typeid(int));
        EXPECT_FALSE(id.is_const());
        EXPECT_FALSE(id.is_volatile());
        EXPECT_FALSE(id.is_reference());
    }

    TEST(type_identifier, format)
    {
        type_identifier id(typeid(int), mge::type_name<int>(), false, false, false);
        type_identifier id_const(typeid(int), mge::type_name<int>(), true, false, false);
        type_identifier id_volatile(typeid(int), mge::type_name<int>(), false, true, false);
        type_identifier id_const_volatile(typeid(int), mge::type_name<int>(), true, true, false);
        type_identifier id_reference(typeid(int), mge::type_name<int>(), false, false, true);
        type_identifier id_const_reference(typeid(int), mge::type_name<int>(), true, false, true);
        type_identifier id_volatile_reference(typeid(int), mge::type_name<int>(), false, true, true);
        type_identifier id_const_volatile_reference(typeid(int),
                                                    mge::type_name<int>(),
                                                    true,
                                                    true,
                                                    true);

        mge::test_format(id, "int");
        mge::test_format(id_const, "const int");
        mge::test_format(id_volatile, "volatile int");
        mge::test_format(id_const_volatile, "const volatile int");
        mge::test_format(id_reference, "int&");
        mge::test_format(id_const_reference, "const int&");
        mge::test_format(id_volatile_reference, "volatile int&");
        mge::test_format(id_const_volatile_reference, "const volatile int&");
    }

    TEST(type_identifier, type_erasure)
    {
        EXPECT_EQ(typeid(int), typeid(const int));
        EXPECT_EQ(typeid(int), typeid(volatile int));
        EXPECT_EQ(typeid(int), typeid(const volatile int));
        EXPECT_EQ(typeid(int), typeid(int&));
    }

    TEST(type_identifier, hash)
    {
        type_identifier id1(typeid(int), mge::type_name<int>(), false, false, false);
        type_identifier id2(typeid(int), mge::type_name<int>(), true, false, false);
        type_identifier id3(typeid(int), mge::type_name<int>(), false, true, false);
        type_identifier id4(typeid(int), mge::type_name<int>(), true, true, false);
        EXPECT_EQ(std::hash<type_identifier>()(id1),
                  std::hash<type_identifier>()(id1));
        EXPECT_NE(std::hash<type_identifier>()(id1),
                  std::hash<type_identifier>()(id2));
        EXPECT_NE(std::hash<type_identifier>()(id1),
                  std::hash<type_identifier>()(id3));
        EXPECT_NE(std::hash<type_identifier>()(id1),
                  std::hash<type_identifier>()(id4));
    }

    TEST(type_identifier, comparison)
    {
        type_identifier id1(typeid(int), mge::type_name<int>(), false, false, false);
        type_identifier id2(typeid(int), mge::type_name<int>(), true, false, false);
        type_identifier id3(typeid(int), mge::type_name<int>(), false, true, false);
        type_identifier id4(typeid(int), mge::type_name<int>(), true, true, false);
        EXPECT_EQ(id1, id1);
        EXPECT_NE(id1, id2);
        EXPECT_NE(id1, id3);
        EXPECT_NE(id1, id4);
    }

    TEST(type_identifier, less_than)
    {
        type_identifier id1(typeid(int), mge::type_name<int>(), false, false, false);
        type_identifier id2(typeid(int), mge::type_name<int>(), true, false, false);
        type_identifier id3(typeid(int), mge::type_name<int>(), false, true, false);
        type_identifier id4(typeid(int), mge::type_name<int>(), true, true, false);
        EXPECT_LT(id1, id2);
        EXPECT_LT(id1, id3);
        EXPECT_LT(id1, id4);
        EXPECT_LT(id2, id4);
        EXPECT_LT(id3, id4);
    }

    TEST(type_identifier, greater_than)
    {
        type_identifier id1(typeid(int), mge::type_name<int>(), false, false, false);
        type_identifier id2(typeid(int), mge::type_name<int>(), true, false, false);
        type_identifier id3(typeid(int), mge::type_name<int>(), false, true, false);
        type_identifier id4(typeid(int), mge::type_name<int>(), true, true, false);
        EXPECT_GT(id2, id1);
        EXPECT_GT(id3, id1);
        EXPECT_GT(id4, id1);
        EXPECT_GT(id4, id2);
        EXPECT_GT(id4, id3);
    }

    TEST(type_identifier, make_type_identifier)
    {
        EXPECT_EQ(make_type_identifier<int>(), make_type_identifier<int>());
        EXPECT_NE(make_type_identifier<int>(),
                  make_type_identifier<const int>());
        EXPECT_NE(make_type_identifier<int>(),
                  make_type_identifier<volatile int>());
        EXPECT_NE(make_type_identifier<int>(),
                  make_type_identifier<const volatile int>());
        EXPECT_EQ(make_type_identifier<const int>(),
                  make_type_identifier<const int>());
        EXPECT_EQ(make_type_identifier<volatile int>(),
                  make_type_identifier<volatile int>());
        EXPECT_EQ(make_type_identifier<const volatile int>(),
                  make_type_identifier<const volatile int>());
        EXPECT_NE(make_type_identifier<int>(), make_type_identifier<int&>());
        EXPECT_NE(make_type_identifier<int&>(),
                  make_type_identifier<const int&>());
        EXPECT_NE(make_type_identifier<int&>(),
                  make_type_identifier<volatile int&>());
        EXPECT_NE(make_type_identifier<int&>(),
                  make_type_identifier<const volatile int&>());
        EXPECT_EQ(make_type_identifier<const int&>(),
                  make_type_identifier<const int&>());
    }

} // namespace mge::reflection
