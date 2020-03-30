// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/script/script_engine.hpp"
#include "test/googletest.hpp"

namespace lua {

    class lua_test : public ::testing::Test
    {
    public:
        static void                   SetUpTestCase();
        static void                   TearDownTestCase();
        static mge::script_engine_ref engine;
    };
} // namespace lua
