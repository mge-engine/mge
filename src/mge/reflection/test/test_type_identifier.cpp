// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/type_identifier.hpp"
#include "test/googletest.hpp"
#include "test/test_format.hpp"
namespace mge::reflection {

    TEST(type_identifier, basic)
    {
        type_identifier id(typeid(int), false, false);
        EXPECT_EQ(id.type_index(), typeid(int));
        EXPECT_FALSE(id.is_const());
        EXPECT_FALSE(id.is_volatile());
    }

    TEST(type_identifier, format)
    {
        type_identifier id(typeid(int), false, false);
        type_identifier id_const(typeid(int), true, false);
        type_identifier id_volatile(typeid(int), false, true);
        type_identifier id_const_volatile(typeid(int), true, true);
        mge::test_format(id, "int");
        mge::test_format(id_const, "const int");
        mge::test_format(id_volatile, "volatile int");
        mge::test_format(id_const_volatile, "const volatile int");

        // type_identifier idptr(typeid(int* const*), false, false);
        // mge::test_format(idptr, "int**");
    }

    TEST(type_identifier, type_erasure)
    {
        EXPECT_EQ(typeid(int), typeid(const int));
        EXPECT_EQ(typeid(int), typeid(volatile int));
        EXPECT_EQ(typeid(int), typeid(const volatile int));
    }


    TEST(type_identifier, hash) {
        type_identifier id1(typeid(int), false, false);
        type_identifier id2(typeid(int), true, false);
        type_identifier id3(typeid(int), false, true);
        type_identifier id4(typeid(int), true, true);
        EXPECT_EQ(std::hash<type_identifier>()(id1), std::hash<type_identifier>()(id1));
        EXPECT_NE(std::hash<type_identifier>()(id1), std::hash<type_identifier>()(id2));
        EXPECT_NE(std::hash<type_identifier>()(id1), std::hash<type_identifier>()(id3));
        EXPECT_NE(std::hash<type_identifier>()(id1), std::hash<type_identifier>()(id4));
    }
} // namespace mge::reflection
