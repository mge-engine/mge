// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "test/googletest.hpp"

#include <string>
using namespace std::literals;

namespace mge::script {

    TEST(module, root)
    {
        mge::script::module m;
        EXPECT_TRUE(m.is_root());
    }

    TEST(module, by_identifier)
    {
        mge::script::module m("mge::script");
        mge::script::module p = m.parent();
        EXPECT_EQ("mge"sv, p.name());
        EXPECT_EQ("script"sv, m.name());
    }

    TEST(module, full_name)
    {
        mge::script::module m("mge::script");
        EXPECT_EQ("::mge::script"sv, m.full_name());
    }

} // namespace mge::script
