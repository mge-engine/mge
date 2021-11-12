// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "test/googletest.hpp"

namespace mge {
    TEST(module, root)
    {
        auto r = mge::script::module::root();
        EXPECT_TRUE(r->is_root());
    }
} // namespace mge
