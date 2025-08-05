// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/module.hpp"
#include "mge/reflection/module_details.hpp"
#include "test/googletest.hpp"

namespace mge::reflection {

    TEST(module_details, root)
    {
        auto root = module_details::root();
        EXPECT_EQ(root->name(), "");
    }

} // namespace mge::reflection