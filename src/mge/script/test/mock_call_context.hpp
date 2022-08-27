// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/call_context.hpp"
#include "test/googlemock.hpp"

class MOCK_call_context : public mge::script::call_context
{
public:
    MOCK_call_context() = default;

    MOCK_METHOD(void*, this_ptr, (), (override));
    MOCK_METHOD(void*, shared_ptr_address, (), (override));

    MOCK_METHOD(void, store_bool_result, (bool), (override));
    MOCK_METHOD(void, store_int8_t_result, (int8_t), (override));
    MOCK_METHOD(void, store_uint8_t_result, (uint8_t), (override));
    MOCK_METHOD(void, store_int16_t_result, (int16_t), (override));
    MOCK_METHOD(void, store_uint16_t_result, (uint16_t), (override));
    MOCK_METHOD(void, store_int32_t_result, (int32_t), (override));
    MOCK_METHOD(void, store_uint32_t_result, (uint32_t), (override));
    MOCK_METHOD(void, store_int64_t_result, (int64_t), (override));
    MOCK_METHOD(void, store_uint64_t_result, (uint64_t), (override));
    MOCK_METHOD(void, store_float_result, (float), (override));
    MOCK_METHOD(void, store_double_result, (double), (override));
    MOCK_METHOD(void, store_string_result, (const std::string&), (override));

    MOCK_METHOD(bool, bool_parameter, (size_t), (override));
    MOCK_METHOD(int8_t, int8_t_parameter, (size_t), (override));
    MOCK_METHOD(uint8_t, uint8_t_parameter, (size_t), (override));
    MOCK_METHOD(int16_t, int16_t_parameter, (size_t), (override));
    MOCK_METHOD(uint16_t, uint16_t_parameter, (size_t), (override));
    MOCK_METHOD(int32_t, int32_t_parameter, (size_t), (override));
    MOCK_METHOD(uint32_t, uint32_t_parameter, (size_t), (override));
    MOCK_METHOD(int64_t, int64_t_parameter, (size_t), (override));
    MOCK_METHOD(uint64_t, uint64_t_parameter, (size_t), (override));
    MOCK_METHOD(float, float_parameter, (size_t), (override));
    MOCK_METHOD(double, double_parameter, (size_t), (override));
    MOCK_METHOD(std::string, string_parameter, (size_t), (override));
    MOCK_METHOD(void*, object_parameter, (size_t, std::type_index), (override));
};