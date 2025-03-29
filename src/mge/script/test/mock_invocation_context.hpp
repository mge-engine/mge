// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/invocation_context.hpp"
#include "test/googlemock.hpp"

class MOCK_invocation_context : public mge::script::invocation_context
{
public:
    MOCK_invocation_context() = default;
    ~MOCK_invocation_context() = default;

    MOCK_METHOD(void, store_bool_argument, (size_t, bool), (override));
    MOCK_METHOD(void, store_int8_t_argument, (size_t, int8_t), (override));
    MOCK_METHOD(void, store_uint8_t_argument, (size_t, uint8_t), (override));
    MOCK_METHOD(void, store_int16_t_argument, (size_t, int16_t), (override));
    MOCK_METHOD(void, store_uint16_t_argument, (size_t, uint16_t), (override));
    MOCK_METHOD(void, store_int32_t_argument, (size_t, int32_t), (override));
    MOCK_METHOD(void, store_uint32_t_argument, (size_t, uint32_t), (override));
    MOCK_METHOD(void, store_int64_t_argument, (size_t, int64_t), (override));
    MOCK_METHOD(void, store_uint64_t_argument, (size_t, uint64_t), (override));
    MOCK_METHOD(void, store_float_argument, (size_t, float), (override));
    MOCK_METHOD(void, store_double_argument, (size_t, double), (override));
    MOCK_METHOD(void,
                store_long_double_argument,
                (size_t, long double),
                (override));
    MOCK_METHOD(void,
                store_string_argument,
                (size_t, const std::string&),
                (override));
    MOCK_METHOD(void,
                store_object_argument,
                (size_t, void*, const mge::script::type_data_ref&),
                (override));

    MOCK_METHOD(call_result_type, call_method, (const char*), (override));

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
