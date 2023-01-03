// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "luatest.hpp"

namespace mge::lua {

    void luatest::SetUp() { engine = mge::script_engine::create("lua"); }

    void luatest::TearDown() { engine.reset(); }

    MGE_REGISTER_IMPLEMENTATION(luatest_application, mge::application, luatest);

} // namespace mge::lua
APPLICATIONTEST_MAIN(luatest);