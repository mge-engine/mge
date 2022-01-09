// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/function_details.hpp"
#include "mge/script/module.hpp"
#include "mock_call_context.hpp"
#include "test/googletest.hpp"
#include <iostream>

using namespace testing;

namespace mge {
    static int voidfunc_value = 0;

    int  intfunc(int x, int y) { return x - y; }
    int  intfunc_noargs(void) { return 42; }
    void voidfunc_intarg(int x) { voidfunc_value = x; }
    void voidfunc() { voidfunc_value = 333; }

    TEST(test_function, intfunc_noargs)
    {
        auto f = mge::script::function("intfunc_noargs", &intfunc_noargs);

        MOCK_call_context ctx;
        EXPECT_CALL(ctx, store_int_result(42)).Times(1);
        f.invoke_function()(ctx);
    }

    TEST(test_function, intfunc)
    {
        auto f = mge::script::function("intfunc", &intfunc);

        MOCK_call_context ctx;
        EXPECT_CALL(ctx, int32_parameter(0)).WillOnce(Return(10));
        EXPECT_CALL(ctx, int32_parameter(1)).WillOnce(Return(3));
        EXPECT_CALL(ctx, store_int_result(7)).Times(1);
        f.invoke_function()(ctx);
    }

    TEST(test_function, voidfunc)
    {
        auto f = mge::script::function("voidfunc", &voidfunc);

        MOCK_call_context ctx;
        f.invoke_function()(ctx);
        EXPECT_EQ(333, voidfunc_value);
    }

} // namespace mge