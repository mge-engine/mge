// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/module.hpp"
#include "mge/reflection/module_details.hpp"
#include "test/googletest.hpp"

namespace mge::reflection {

    TEST(module_details, root)
    {
        auto root_details = module_details::root();
        EXPECT_EQ(root_details->name(), "");
        EXPECT_TRUE(root_details->is_root());

        auto root = module();
        EXPECT_TRUE(root.is_root());
    }

    TEST(module, construct)
    {
        mge::reflection::module mod;
        EXPECT_TRUE(mod.is_root());
        mge::reflection::module mod2("::");
        EXPECT_TRUE(mod2.is_root());

        mge::reflection::module mod3("::test");
        EXPECT_EQ(mod3.name(), "test");
        EXPECT_EQ(mod3.full_name(), "::test");
        EXPECT_FALSE(mod3.is_root());

        mge::reflection::module mod4("::test::submodule");
        EXPECT_EQ(mod4.name(), "submodule");
        EXPECT_EQ(mod4.full_name(), "::test::submodule");
        EXPECT_FALSE(mod4.is_root());

        auto mod4parent = mod4.parent();
        EXPECT_EQ(mod4parent.name(), "test");
        EXPECT_EQ(mod4parent.full_name(), "::test");
        EXPECT_FALSE(mod4parent.is_root());
    }

} // namespace mge::reflection