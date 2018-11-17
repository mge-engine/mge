// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/type.hpp"

enum E1 {
    EVAL1,
    EVAL2
};


TEST(type_enum, register_it)
{
    auto& t = mge::reflection::type::get<E1>();
    EXPECT_EQ(std::string("E1"), t.name());
}

TEST(type_enum, register_it_add_values)
{
    mge::reflection::type::get<E1>()
      .enum_value("EVAL1", EVAL1)
      .enum_value("EVAL2", EVAL2);
}
