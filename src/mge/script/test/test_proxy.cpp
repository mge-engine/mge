// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/implement_method.hpp"
#include "mge/script/module.hpp"
#include "mge/script/proxy.hpp"
#include "mge/script/type.hpp"
#include "mock_invocation_context.hpp"
#include "test/googletest.hpp"

class test_interface
{
public:
    virtual ~test_interface() = default;
    virtual int test_method(int a, int b) = 0;
    virtual int test_int_method() = 0;
};

class test_proxy : public mge::script::proxy<test_interface>
{
public:
    test_proxy() = default;

    MGE_IMPLEMENT_METHOD(int, test_int_method, (), override);
    MGE_IMPLEMENT_METHOD(int, test_method, (int, int), override);
};

TEST(proxy, call_no_args_throws_if_no_context_set)
{
    test_proxy p;
    EXPECT_THROW(p.test_int_method(), mge::illegal_state);
}

TEST(proxy, call_no_args_produces_result)
{
    test_proxy              p;
    MOCK_invocation_context ctx;
    p.set_context(&ctx);
    EXPECT_CALL(ctx, call_method("test_int_method"))
        .WillOnce(testing::Return(
            mge::script::invocation_context::call_result_type::CALL_EXECUTED));
    EXPECT_CALL(ctx, get_int32_t_result()).WillOnce(testing::Return(42));
    EXPECT_EQ(42, p.test_int_method());
}
