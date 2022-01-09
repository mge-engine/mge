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

    MOCK_METHOD(void, store_bool_result, (bool), (override));
    MOCK_METHOD(void, store_char_result, (char), (override));
    MOCK_METHOD(void, store_signed_char_result, (signed char), (override));
    MOCK_METHOD(void, store_unsigned_char_result, (unsigned char), (override));
    MOCK_METHOD(void, store_short_result, (short), (override));
    MOCK_METHOD(void,
                store_unsigned_short_result,
                (unsigned short),
                (override));
    MOCK_METHOD(void, store_int_result, (int), (override));
    MOCK_METHOD(void, store_unsigned_int_result, (unsigned int), (override));
    MOCK_METHOD(void, store_long_result, (long), (override));
    MOCK_METHOD(void, store_unsigned_long_result, (unsigned long), (override));
    MOCK_METHOD(void, store_int64_result, (int64_t), (override));
    MOCK_METHOD(void, store_uint64_result, (uint64_t), (override));
    MOCK_METHOD(void, store_float_result, (float), (override));
    MOCK_METHOD(void, store_double_result, (double), (override));
    MOCK_METHOD(void, store_string_result, (const std::string&), (override));

    MOCK_METHOD(bool, bool_parameter, (size_t), (override));
    MOCK_METHOD(char, int8_parameter, (size_t), (override));
    MOCK_METHOD(unsigned char, uint8_parameter, (size_t), (override));
    MOCK_METHOD(short, int16_parameter, (size_t), (override));
    MOCK_METHOD(unsigned short, uint16_parameter, (size_t), (override));
    MOCK_METHOD(int, int32_parameter, (size_t), (override));
    MOCK_METHOD(unsigned int, uint32_parameter, (size_t), (override));
    MOCK_METHOD(int64_t, int64_parameter, (size_t), (override));
    MOCK_METHOD(uint64_t, uint64_parameter, (size_t), (override));
    MOCK_METHOD(float, float_parameter, (size_t), (override));
    MOCK_METHOD(double, double_parameter, (size_t), (override));
    MOCK_METHOD(std::string, string_parameter, (size_t), (override));
    MOCK_METHOD(void*, object_parameter, (size_t, std::type_index), (override));
};