// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/traits.hpp"
#include "test/googletest.hpp"

namespace mge::script {
    TEST(traits, is_void)
    {
        static_assert(mge::script::traits_of<void>().is_void);

        auto t_void = mge::script::traits_of<void>();
        EXPECT_TRUE(t_void.is_void);

        auto t = mge::script::traits_of<int>();
        EXPECT_FALSE(t.is_void);
    }
} // namespace mge::script
