#pragma once

#include "test/googletest.hpp"
#include "mge/script/script_engine.hpp"

namespace lua {

    class lua_test : public ::testing::Test
    {
    public:
        static void SetUpTestCase();
        static void TearDownTestCase();
        static mge::script_engine_ref engine;
    };
}
