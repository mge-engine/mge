// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"

#include "test/googletest.hpp"

TEST(type, bool)
{
    using namespace mge::script;

    mge::script::module m;
    const auto&         t = m.type("bool");
    EXPECT_EQ("bool", t.name());
}