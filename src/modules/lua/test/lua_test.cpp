#include "lua_test.hpp"

namespace lua {

    mge::script_engine_ref lua_test::engine;

    void
    lua_test::SetUpTestCase()
    {
        engine = mge::script_engine::create("lua");
        EXPECT_TRUE(engine.get() != nullptr);
    }

    void
    lua_test::TearDownTestCase()
    {
        engine.reset();
    }
}
