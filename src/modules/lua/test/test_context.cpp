// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "luatest.hpp"

namespace mge::lua {
    class test_context : public luatest
    {};

    TEST_F(test_context, create) { auto context = engine->create_context(); }

} // namespace mge::lua