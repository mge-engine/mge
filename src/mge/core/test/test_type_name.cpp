// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/type_name.hpp"
#include "test/googletest.hpp"

TEST(type_name, simple_name)
{
    auto tn = std::string(mge::type_name<int>());
    EXPECT_STREQ("int", tn.c_str());
}

class foo
{};

TEST(type_name, simple_class_name)
{
    auto tn = std::string(mge::type_name<foo>());
    EXPECT_STREQ("foo", tn.c_str());
}

struct bar
{};

TEST(type_name, simple_struct_name)
{
    auto tn = std::string(mge::type_name<bar>());
    EXPECT_STREQ("bar", tn.c_str());
}

TEST(type_name, pointer_name)
{
    auto tn = std::string(mge::type_name<bar**>());
#if defined(MGE_COMPILER_MSVC)
    EXPECT_STREQ("bar * *", tn.c_str());
#elif defined(MGE_COMPILER_GCC)
    EXPECT_STREQ("bar**", tn.c_str());
#endif
}

TEST(type_name, complex_name)
{
    auto tn =
        std::string(mge::type_name<std::remove_reference_t<decltype(*this)>>());
    EXPECT_STREQ("type_name_complex_name_Test", tn.c_str());
}

TEST(type_name, signed_char)
{
    auto tn = std::string(mge::type_name<signed char>());
    EXPECT_STREQ("signed char", tn.c_str());
}
