// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "test/googlemock.hpp"

class MOCK_call_context : public mge::script::call_context
{
public:
    MOCK_call_context() = default;
    ~MOCK_call_context() = default;

    MOCK_METHOD(void*, this_ptr, (), (override));
    MOCK_METHOD(void*, shared_ptr_address, (), (override));

    MOCK_METHOD(void, bool_result, (bool), (override));
    MOCK_METHOD(void, int8_t_result, (int8_t), (override));
    MOCK_METHOD(void, int16_t_result, (int16_t), (override));
    MOCK_METHOD(void, int32_t_result, (int32_t), (override));
    MOCK_METHOD(void, int64_t_result, (int64_t), (override));
    MOCK_METHOD(void, uint8_t_result, (uint8_t), (override));
    MOCK_METHOD(void, uint16_t_result, (uint16_t), (override));
    MOCK_METHOD(void, uint32_t_result, (uint32_t), (override));
    MOCK_METHOD(void, uint64_t_result, (uint64_t), (override));
    MOCK_METHOD(void, float_result, (float), (override));
    MOCK_METHOD(void, double_result, (double), (override));
    MOCK_METHOD(void, long_double_result, (long double), (override));
    MOCK_METHOD(void, string_result, (const std::string&), (override));
    MOCK_METHOD(void, wstring_result, (const std::wstring&), (override));
    MOCK_METHOD(void,
                object_result,
                (void*, const mge::script::type_data_ref&),
                ());

    MOCK_METHOD(void, exception_thrown_mge, (const mge::exception&), ());
    MOCK_METHOD(void, exception_thrown_std, (const std::exception&), ());
    MOCK_METHOD(void, exception_thrown_noargs, (), ());

    virtual void exception_thrown(const mge::exception& e) override
    {
        exception_thrown_mge(e);
    }

    virtual void exception_thrown(const std::exception& e) override
    {
        exception_thrown_std(e);
    }

    virtual void exception_thrown() override
    {
        exception_thrown_noargs();
    }

    MOCK_METHOD(bool, bool_parameter, (size_t), ());
    MOCK_METHOD(int8_t, int8_t_parameter, (size_t), ());
    MOCK_METHOD(int16_t, int16_t_parameter, (size_t), ());
    MOCK_METHOD(int32_t, int32_t_parameter, (size_t), ());
    MOCK_METHOD(int64_t, int64_t_parameter, (size_t), ());
    MOCK_METHOD(uint8_t, uint8_t_parameter, (size_t), ());
    MOCK_METHOD(uint16_t, uint16_t_parameter, (size_t), ());
    MOCK_METHOD(uint32_t, uint32_t_parameter, (size_t), ());
    MOCK_METHOD(uint64_t, uint64_t_parameter, (size_t), ());
    MOCK_METHOD(float, float_parameter, (size_t), ());
    MOCK_METHOD(double, double_parameter, (size_t), ());
    MOCK_METHOD(long double, long_double_parameter, (size_t), ());
    MOCK_METHOD(std::string, string_parameter, (size_t), ());

    MOCK_METHOD(void, before_call, (), ());
    MOCK_METHOD(void, after_call, (), ());
};
