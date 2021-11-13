// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"
#include "test/googletest.hpp"

#include <string_view>

using namespace std::literals;

namespace mge {

    TEST(type, void_type)
    {
        auto t = mge::script::type<void>();
        EXPECT_EQ("void"sv, t.name());
    }

} // namespace mge
