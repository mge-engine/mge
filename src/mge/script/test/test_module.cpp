// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "test/googletest.hpp"

TEST(module, root)
{
    mge::script::module m;
    EXPECT_TRUE(m.is_root());
}

TEST(module, submodule)
{
    using mge::script::module;

    mge::script::module m("mge::script");
    auto                p = m.parent();
    EXPECT_EQ("mge", p.name());
    EXPECT_EQ("script", m.name());
}
