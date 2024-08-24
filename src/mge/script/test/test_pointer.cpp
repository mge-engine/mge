// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"

#include "test/googletest.hpp"

TEST(pointer, create)
{
    using namespace mge::script;

    mge::script::module m;
    m(type<int*>());

    const auto& t = m.type("int*");
    EXPECT_EQ("int*", t.name());
    EXPECT_EQ("int", t.pointer_specific().pointee->name());
    EXPECT_FALSE(t.pointer_specific().is_const);
    EXPECT_FALSE(t.pointer_specific().is_volatile);
}