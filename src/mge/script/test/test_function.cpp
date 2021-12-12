// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/bind.hpp"
#include "mge/script/function.hpp"
#include "mge/script/function_details.hpp"
#include "mge/script/module.hpp"
#include "mock_call_context.hpp"
#include "test/googletest.hpp"
#include <iostream>

using namespace testing;

namespace mge {
    class test_function : public ::testing::Test
    {
    public:
        void SetUp() override { mge::script::bind_all(); }
    };

    int intfunc(int x, int y) { return x - y; }
    int intfunc_noargs(void) { return 42; }

    TEST_F(test_function, intfunc_noargs)
    {
        mge::script::function f("intfunc_noargs", &intfunc_noargs);

        auto              invoke = f.invoke();
        MOCK_call_context ctx;
        EXPECT_CALL(ctx, store_int_result(42)).Times(1);
        invoke(ctx);
    }

    TEST_F(test_function, intfunc)
    {
        mge::script::function f("intfunc", &intfunc);

        auto              invoke = f.invoke();
        MOCK_call_context ctx;
        EXPECT_CALL(ctx, int_parameter(0)).WillOnce(Return(10));
        EXPECT_CALL(ctx, int_parameter(1)).WillOnce(Return(3));
        EXPECT_CALL(ctx, store_int_result(7)).Times(1);

        invoke(ctx);
    }
} // namespace mge