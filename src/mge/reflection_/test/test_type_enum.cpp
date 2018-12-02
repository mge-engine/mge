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
    mge::reflection::type<E1> t;
    EXPECT_EQ(std::string("E1"), t.name());
}
#if 0
TEST(type_enum, register_it_add_values)
{
    type<E1>()
      .enum_value("EVAL1", EVAL1)
      .enum_value("EVAL2", EVAL2);
}
#endif
