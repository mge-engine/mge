// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mock_call_context.hpp"
#include "test/googletest.hpp"

using namespace testing;

int  test_function_void_called = 0;
void test_function_void(void) { test_function_void_called = 42; }
void test_function_void_throw_mge_exception(void)
{
    MGE_THROW(mge::exception) << "test";
}
void test_function_void_throw_std_exception(void) { throw std::exception(); }
void test_function_void_throw_int_exception(void) { throw 42; }

TEST(function, void_c_function)
{
    using mge::script::function;

    mge::script::module m;
    m(function("test_function_void", test_function_void));
    m(function("test_function_void_throw_mge_exception",
               test_function_void_throw_mge_exception));
    m(function("test_function_void_throw_std_exception",
               test_function_void_throw_std_exception));
    m(function("test_function_void_throw_int_exception",
               test_function_void_throw_int_exception));
    MOCK_call_context ctx;
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    m.function("test_function_void").invoke(ctx);
    EXPECT_EQ(42, test_function_void_called);
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    EXPECT_CALL(ctx, exception_thrown_mge(_)).Times(1);
    m.function("test_function_void_throw_mge_exception").invoke(ctx);
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    EXPECT_CALL(ctx, exception_thrown_std(_)).Times(1);
    m.function("test_function_void_throw_std_exception").invoke(ctx);
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    EXPECT_CALL(ctx, exception_thrown_noargs()).Times(1);
    m.function("test_function_void_throw_int_exception").invoke(ctx);
}

int8_t test_function_int8_t() { return 42; }
int8_t test_function_int8_t_throw_mge_exception(void)
{
    MGE_THROW(mge::exception) << "test";
}

TEST(function, int8_t_c_function)
{
    using mge::script::function;

    mge::script::module m;
    m(function("test_function_int8_t", test_function_int8_t));
    m(function("test_function_int8_t_throw_mge_exception",
               test_function_int8_t_throw_mge_exception));
    MOCK_call_context ctx;
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    EXPECT_CALL(ctx, int8_t_result(42)).Times(1);
    m.function("test_function_int8_t").invoke(ctx);
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    EXPECT_CALL(ctx, exception_thrown_mge(_)).Times(1);
    m.function("test_function_int8_t_throw_mge_exception").invoke(ctx);
}

int32_t test_function_sum(int32_t a, int32_t b) { return a + b; }

TEST(function, test_function_sum)
{
    using mge::script::function;

    mge::script::module m;
    m(function("test_function_sum", test_function_sum));
    MOCK_call_context ctx;
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    EXPECT_CALL(ctx, int32_t_result(42)).Times(1);
    EXPECT_CALL(ctx, int32_t_parameter(0)).WillOnce(Return(20));
    EXPECT_CALL(ctx, int32_t_parameter(1)).WillOnce(Return(22));
    m.function("test_function_sum").invoke(ctx);
}