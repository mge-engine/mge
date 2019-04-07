// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "lua_test.hpp"
#include "mge/script/script_context.hpp"
#include "mge/core/exception.hpp"
namespace lua {

    class test_context : public lua_test
    {};

    TEST_F(test_context, create)
    {
        auto ctx = engine->create_context();
        EXPECT_TRUE(ctx.get() != nullptr);
    }

    TEST_F(test_context, simple_eval)
    {
        auto ctx = engine->create_context();
        ctx->eval("i = 1\n"
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
        } catch(const mge::exception& ex) {
            EXPECT_TRUE(strstr(ex.what(), "attempt to compare boolean with number") != nullptr);
        }
    }

}
