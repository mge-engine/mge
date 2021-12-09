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

    int func(int x, int y) { return x - y; }

    TEST_F(test_function, simple_int)
    {
        mge::script::function f("func", &func);

        auto              invoke = f.invoke();
        MOCK_call_context ctx;
        EXPECT_CALL(ctx, int_parameter(0)).WillOnce(Return(10));
        EXPECT_CALL(ctx, int_parameter(1)).WillOnce(Return(3));
        EXPECT_CALL(ctx, store_int_result(7)).Times(1);

        invoke(ctx);
    }
} // namespace mge