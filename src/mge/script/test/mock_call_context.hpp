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
    MOCK_METHOD(void, store_long_result, (int64_t), (override));
    MOCK_METHOD(void, store_unsigned_long_result, (uint64_t), (override));
    MOCK_METHOD(void, store_float_result, (float), (override));
    MOCK_METHOD(void, store_double_result, (double), (override));
    MOCK_METHOD(void, store_string_result, (const std::string&), (override));
    MOCK_METHOD(bool, bool_parameter, (size_t), (override));
    MOCK_METHOD(char, char_parameter, (size_t), (override));
    MOCK_METHOD(unsigned char, unsigned_char_parameter, (size_t), (override));
    MOCK_METHOD(short, short_parameter, (size_t), (override));
    MOCK_METHOD(unsigned short, unsigned_short_parameter, (size_t), (override));
    MOCK_METHOD(int, int_parameter, (size_t), (override));
    MOCK_METHOD(unsigned int, unsigned_int_parameter, (size_t), (override));
    MOCK_METHOD(int64_t, long_parameter, (size_t), (override));
    MOCK_METHOD(uint64_t, unsigned_long_parameter, (size_t), (override));
    MOCK_METHOD(float, float_parameter, (size_t), (override));
    MOCK_METHOD(double, double_parameter, (size_t), (override));
    MOCK_METHOD(std::string, string_parameter, (size_t), (override));
    MOCK_METHOD(void*, object_parameter, (size_t, std::type_index), (override));
};