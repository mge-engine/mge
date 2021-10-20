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

} // namespace mge::lua