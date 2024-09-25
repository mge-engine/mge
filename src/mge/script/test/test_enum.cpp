// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"

#include "test/googletest.hpp"

enum test_enum
{
    TEST1,
    TEST2
};

TEST(enum, create)
{
    using namespace mge::script;

    mge::script::module m;
    m(type<test_enum>());

    const auto& t = m.type("test_enum");
    EXPECT_EQ("test_enum", t.name());
    EXPECT_EQ("int", t.enum_specific().underlying_type->name());
    EXPECT_EQ(2, t.enum_specific().values.size());
    EXPECT_EQ("TEST1", t.enum_specific().values[0].second);
    EXPECT_EQ(0, t.enum_specific().values[0].first);
    EXPECT_EQ("TEST2", t.enum_specific().values[1].second);
    EXPECT_EQ(1, t.enum_specific().values[1].first);
}