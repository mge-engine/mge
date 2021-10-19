// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "luatest.hpp"

namespace mge::lua {
    class test_context : public luatest
    {};

    TEST_F(test_context, create) { auto context = engine->create_context(); }

    TEST_F(test_context, simple_eval)
    {
        auto context = engine->create_context();
        context->eval("i = 1\n"
                      "while i < 3 do\n"
                      "  print(i)\n"
                      "  i = i + 1\n"
                      "end");
    }

    TEST_F(test_context, simple_eval_throws_on_syntax_error)
    {
        auto ctx = engine->create_context();
        try {
            ctx->eval("i = 1\n"
                      "while i < 3 <3 >5 do\n"
                      "  print(i)\n"
                      "  i = i + 1\n"
                      "end");
            FAIL() << "Exception expected";
        } catch (const mge::exception& ex) {
            EXPECT_TRUE(
                strstr(ex.what(), "attempt to compare boolean with number") !=
                nullptr);
        }
    }

} // namespace mge::lua