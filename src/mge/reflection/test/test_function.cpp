// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mock_call_context.hpp"
#include "test/googletest.hpp"

namespace mge::reflection {

    namespace {
        int add(int a, int b)
        {
            return a + b;
        }

        void no_op() noexcept {}

        int multiply(int a, int b) noexcept
        {
            return a * b;
        }

        void print_hello()
        {
            // Does nothing in test
        }

        int subtract(int a, int b)
        {
            return a - b;
        }

        int divide(int a, int b) noexcept
        {
            return a / b;
        }
    } // namespace

    TEST(function, constructor_with_return_and_parameters)
    {
        function f("add", add);

        const auto& details = f.details();
        ASSERT_TRUE(details);
        EXPECT_EQ(details->name(), "add");
        EXPECT_FALSE(details->is_noexcept());

        const auto& sig = details->signature();
        EXPECT_EQ(sig.return_type(), make_type_identifier<int>());
        ASSERT_EQ(sig.parameter_types().size(), 2);
        EXPECT_EQ(sig.parameter_types()[0], make_type_identifier<int>());
        EXPECT_EQ(sig.parameter_types()[1], make_type_identifier<int>());
    }

    TEST(function, constructor_noexcept_void_no_params)
    {
        function f("no_op", no_op);

        const auto& details = f.details();
        ASSERT_TRUE(details);
        EXPECT_EQ(details->name(), "no_op");
        EXPECT_TRUE(details->is_noexcept());

        const auto& sig = details->signature();
        EXPECT_EQ(sig.return_type(), make_type_identifier<void>());
        EXPECT_TRUE(sig.parameter_types().empty());
    }

    TEST(function, constructor_noexcept_with_return_and_parameters)
    {
        function f("multiply", multiply);

        const auto& details = f.details();
        ASSERT_TRUE(details);
        EXPECT_EQ(details->name(), "multiply");
        EXPECT_TRUE(details->is_noexcept());

        const auto& sig = details->signature();
        EXPECT_EQ(sig.return_type(), make_type_identifier<int>());
        ASSERT_EQ(sig.parameter_types().size(), 2);
        EXPECT_EQ(sig.parameter_types()[0], make_type_identifier<int>());
        EXPECT_EQ(sig.parameter_types()[1], make_type_identifier<int>());
    }

    TEST(function, constructor_void_no_params)
    {
        function f("print_hello", print_hello);

        const auto& details = f.details();
        ASSERT_TRUE(details);
        EXPECT_EQ(details->name(), "print_hello");
        EXPECT_FALSE(details->is_noexcept());

        const auto& sig = details->signature();
        EXPECT_EQ(sig.return_type(), make_type_identifier<void>());
        EXPECT_TRUE(sig.parameter_types().empty());
    }

    TEST(function, invoke_noexcept_void_no_params)
    {
        static bool called = false;
        called = false;

        auto     test_func = []() noexcept { called = true; };
        function f("test_func", +test_func);

        MOCK_call_context ctx;
        f.details()->invoke(ctx);

        EXPECT_TRUE(called);
    }

    TEST(function, invoke_noexcept_with_return_value)
    {
        function f("multiply", multiply);

        MOCK_call_context ctx;
        EXPECT_CALL(ctx, int32_t_parameter(1)).WillOnce(testing::Return(3));
        EXPECT_CALL(ctx, int32_t_parameter(0)).WillOnce(testing::Return(7));
        EXPECT_CALL(ctx, int32_t_result(21)).Times(1);

        f.details()->invoke(ctx);
    }

    TEST(function, invoke_void_no_params)
    {
        static bool called = false;
        called = false;

        auto     test_func = []() { called = true; };
        function f("test_func", +test_func);

        MOCK_call_context ctx;
        f.details()->invoke(ctx);

        EXPECT_TRUE(called);
    }

    TEST(function, invoke_with_exception_handling)
    {
        auto throwing_func = []() {
            throw std::runtime_error("Test exception");
        };
        function f("throwing_func", +throwing_func);

        MOCK_call_context ctx;
        EXPECT_CALL(ctx, exception_thrown(testing::A<const std::exception&>()))
            .Times(1);

        f.details()->invoke(ctx);
    }

    TEST(function, constructor_std_function_with_return_and_parameters)
    {
        std::function<int(int, int)> func = [](int a, int b) { return a + b; };
        function                     f("std_add", func);

        const auto& details = f.details();
        ASSERT_TRUE(details);
        EXPECT_EQ(details->name(), "std_add");
        EXPECT_FALSE(details->is_noexcept());

        const auto& sig = details->signature();
        EXPECT_EQ(sig.return_type(), make_type_identifier<int>());
        ASSERT_EQ(sig.parameter_types().size(), 2);
        EXPECT_EQ(sig.parameter_types()[0], make_type_identifier<int>());
        EXPECT_EQ(sig.parameter_types()[1], make_type_identifier<int>());
    }

    TEST(function, constructor_std_function_void_no_params)
    {
        std::function<void()> func = []() {};
        function              f("std_void", func);

        const auto& details = f.details();
        ASSERT_TRUE(details);
        EXPECT_EQ(details->name(), "std_void");
        EXPECT_FALSE(details->is_noexcept());

        const auto& sig = details->signature();
        EXPECT_EQ(sig.return_type(), make_type_identifier<void>());
        EXPECT_TRUE(sig.parameter_types().empty());
    }

    TEST(function, invoke_std_function_with_return_value)
    {
        std::function<int(int, int)> func = [](int a, int b) { return a * b; };
        function                     f("std_multiply", func);

        MOCK_call_context ctx;
        EXPECT_CALL(ctx, int32_t_parameter(1)).WillOnce(testing::Return(4));
        EXPECT_CALL(ctx, int32_t_parameter(0)).WillOnce(testing::Return(5));
        EXPECT_CALL(ctx, int32_t_result(20)).Times(1);

        f.details()->invoke(ctx);
    }

    TEST(function, invoke_std_function_void_no_params)
    {
        static bool called = false;
        called = false;

        std::function<void()> func = []() { called = true; };
        function              f("std_call", func);

        MOCK_call_context ctx;
        f.details()->invoke(ctx);

        EXPECT_TRUE(called);
    }

    TEST(function, invoke_std_function_with_exception_handling)
    {
        std::function<void()> func = []() {
            throw std::runtime_error("Test exception from std::function");
        };
        function f("std_throwing", func);

        MOCK_call_context ctx;
        EXPECT_CALL(ctx, exception_thrown(testing::A<const std::exception&>()))
            .Times(1);

        f.details()->invoke(ctx);
    }

    TEST(function, invoke_parameter_order_matters)
    {
        function f("subtract", subtract);

        MOCK_call_context ctx;
        // Parameters should be retrieved in reverse order (index 1, then 0)
        // subtract(10, 3) should return 7, not -7
        EXPECT_CALL(ctx, int32_t_parameter(1)).WillOnce(testing::Return(3));
        EXPECT_CALL(ctx, int32_t_parameter(0)).WillOnce(testing::Return(10));
        EXPECT_CALL(ctx, int32_t_result(7)).Times(1);

        f.details()->invoke(ctx);
    }

    TEST(function, invoke_noexcept_parameter_order_matters)
    {
        function f("divide", divide);

        MOCK_call_context ctx;
        // Parameters should be retrieved in reverse order (index 1, then 0)
        // divide(20, 4) should return 5, not 0
        EXPECT_CALL(ctx, int32_t_parameter(1)).WillOnce(testing::Return(4));
        EXPECT_CALL(ctx, int32_t_parameter(0)).WillOnce(testing::Return(20));
        EXPECT_CALL(ctx, int32_t_result(5)).Times(1);

        f.details()->invoke(ctx);
    }

    TEST(function, invoke_std_function_parameter_order_matters)
    {
        std::function<int(int, int)> func = [](int a, int b) { return a - b; };
        function                     f("std_subtract", func);

        MOCK_call_context ctx;
        // Parameters should be retrieved in reverse order (index 1, then 0)
        // subtract(15, 5) should return 10, not -10
        EXPECT_CALL(ctx, int32_t_parameter(1)).WillOnce(testing::Return(5));
        EXPECT_CALL(ctx, int32_t_parameter(0)).WillOnce(testing::Return(15));
        EXPECT_CALL(ctx, int32_t_result(10)).Times(1);

        f.details()->invoke(ctx);
    }

} // namespace mge::reflection
