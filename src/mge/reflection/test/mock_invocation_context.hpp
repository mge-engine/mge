// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/invocation_context.hpp"
#include "test/googlemock.hpp"

namespace mge::reflection {

    class MOCK_invocation_context_base : public invocation_context
    {
    public:
        MOCK_invocation_context_base() = default;
        virtual ~MOCK_invocation_context_base() = default;

        MOCK_METHOD(bool, call_implemented, (const char* method), (override));

        MOCK_METHOD(void,
                    store_bool_argument,
                    (size_t index, bool value),
                    (override));
        MOCK_METHOD(void,
                    store_int8_t_argument,
                    (size_t index, int8_t value),
                    (override));
        MOCK_METHOD(void,
                    store_uint8_t_argument,
                    (size_t index, uint8_t value),
                    (override));
        MOCK_METHOD(void,
                    store_int16_t_argument,
                    (size_t index, int16_t value),
                    (override));
        MOCK_METHOD(void,
                    store_uint16_t_argument,
                    (size_t index, uint16_t value),
                    (override));
        MOCK_METHOD(void,
                    store_int32_t_argument,
                    (size_t index, int32_t value),
                    (override));
        MOCK_METHOD(void,
                    store_uint32_t_argument,
                    (size_t index, uint32_t value),
                    (override));
        MOCK_METHOD(void,
                    store_int64_t_argument,
                    (size_t index, int64_t value),
                    (override));
        MOCK_METHOD(void,
                    store_uint64_t_argument,
                    (size_t index, uint64_t value),
                    (override));
        MOCK_METHOD(void,
                    store_float_argument,
                    (size_t index, float value),
                    (override));
        MOCK_METHOD(void,
                    store_double_argument,
                    (size_t index, double value),
                    (override));
        MOCK_METHOD(void,
                    store_long_double_argument,
                    (size_t index, long double value),
                    (override));
        MOCK_METHOD(void,
                    store_string_argument,
                    (size_t index, const std::string& value),
                    (override));

        MOCK_METHOD(invocation_context::call_result_type,
                    call_method,
                    (const char* method),
                    (override));

        MOCK_METHOD(bool, get_bool_result, (), (override));
        MOCK_METHOD(int8_t, get_int8_t_result, (), (override));
        MOCK_METHOD(uint8_t, get_uint8_t_result, (), (override));
        MOCK_METHOD(int16_t, get_int16_t_result, (), (override));
        MOCK_METHOD(uint16_t, get_uint16_t_result, (), (override));
        MOCK_METHOD(int32_t, get_int32_t_result, (), (override));
        MOCK_METHOD(uint32_t, get_uint32_t_result, (), (override));
        MOCK_METHOD(int64_t, get_int64_t_result, (), (override));
        MOCK_METHOD(uint64_t, get_uint64_t_result, (), (override));
        MOCK_METHOD(float, get_float_result, (), (override));
        MOCK_METHOD(double, get_double_result, (), (override));
        MOCK_METHOD(long double, get_long_double_result, (), (override));
        MOCK_METHOD(std::string, get_string_result, (), (override));
    };

    using MOCK_invocation_context =
        testing::StrictMock<MOCK_invocation_context_base>;

} // namespace mge::reflection
