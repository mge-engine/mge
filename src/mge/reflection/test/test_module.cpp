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
    }

} // namespace mge::reflection