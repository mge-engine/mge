// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/type_name.hpp"
#include "test/googletest.hpp"

TEST(type_name, simple_name)
{
    auto tn = mge::type_name<int>();
    EXPECT_STREQ("int", tn.c_str());
}

class foo
{};

TEST(type_name, simple_class_name)
{
    auto tn = mge::type_name<foo>();
    EXPECT_STREQ("foo", tn.c_str());
}

struct bar
{};

TEST(type_name, simple_struct_name)
{
    auto tn = mge::type_name<bar>();
    EXPECT_STREQ("bar", tn.c_str());
}

TEST(type_name, pointer_name)
{
    auto tn = mge::type_name<bar**>();
    EXPECT_STREQ("bar**", tn.c_str());
}

TEST(type_name, complex_name)
{
    auto tn = mge::type_name<decltype(*this)>();
    EXPECT_STREQ("type_name_complex_name_Test", tn.c_str());
}

TEST(type_name, signed_char)
{
    auto tn = mge::type_name<signed char>();
    EXPECT_STREQ("signed char", tn.c_str());
}
