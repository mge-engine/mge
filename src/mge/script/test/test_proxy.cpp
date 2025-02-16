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

TEST(proxy, check_MGE_INTERNAL_COUNT)
{
    EXPECT_EQ(1, MGE_INTERNAL_COUNT_ARGS((int a, int b)));
    EXPECT_EQ(0, MGE_INTERNAL_COUNT_ARGS());
    EXPECT_EQ(1, MGE_INTERNAL_COUNT_ARGS(a));
    EXPECT_EQ(2, MGE_INTERNAL_COUNT_ARGS(a, b));
    EXPECT_EQ(3, MGE_INTERNAL_COUNT_ARGS(a, b, c));
    EXPECT_EQ(4, MGE_INTERNAL_COUNT_ARGS(a, b, c, d));
    EXPECT_EQ(5, MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e));
    EXPECT_EQ(6, MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f));
    EXPECT_EQ(7, MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f, g));
    EXPECT_EQ(8, MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f, g, h));
    EXPECT_EQ(9, MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f, g, h, i));
    EXPECT_EQ(10, MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f, g, h, i, j));
    EXPECT_EQ(11, MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f, g, h, i, j, k));
    EXPECT_EQ(12, MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f, g, h, i, j, k, l));
    EXPECT_EQ(13,
              MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f, g, h, i, j, k, l, m));
    EXPECT_EQ(
        14,
        MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f, g, h, i, j, k, l, m, n));
    EXPECT_EQ(
        15,
        MGE_INTERNAL_COUNT_ARGS(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o));
    EXPECT_EQ(16,
              MGE_INTERNAL_COUNT_ARGS(a,
                                      b,
                                      c,
                                      d,
                                      e,
                                      f,
                                      g,
                                      h,
                                      i,
                                      j,
                                      k,
                                      l,
                                      m,
                                      n,
                                      o,
                                      p));
    EXPECT_EQ(17,
              MGE_INTERNAL_COUNT_ARGS(a,
                                      b,
                                      c,
                                      d,
                                      e,
                                      f,
                                      g,
                                      h,
                                      i,
                                      j,
                                      k,
                                      l,
                                      m,
                                      n,
                                      o,
                                      p,
                                      q));
    EXPECT_EQ(18,
              MGE_INTERNAL_COUNT_ARGS(a,
                                      b,
                                      c,
                                      d,
                                      e,
                                      f,
                                      g,
                                      h,
                                      i,
                                      j,
                                      k,
                                      l,
                                      m,
                                      n,
                                      o,
                                      p,
                                      q,
                                      r));
    EXPECT_EQ(19,
              MGE_INTERNAL_COUNT_ARGS(a,
                                      b,
                                      c,
                                      d,
                                      e,
                                      f,
                                      g,
                                      h,
                                      i,
                                      j,
                                      k,
                                      l,
                                      m,
                                      n,
                                      o,
                                      p,
                                      q,
                                      r,
                                      s));
}