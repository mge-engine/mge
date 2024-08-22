// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "test/googlemock.hpp"

class MOCK_call_context : public mge::script::call_context
{
public:
    MOCK_call_context() {}
    ~MOCK_call_context() {}

    MOCK_METHOD(void, store_bool_result, (bool), ());
    MOCK_METHOD(void, store_int8_t_result, (int8_t), ());
    MOCK_METHOD(void, store_int16_t_result, (int16_t), ());
    MOCK_METHOD(void, store_int32_t_result, (int32_t), ());
    MOCK_METHOD(void, store_int64_t_result, (int64_t), ());
    MOCK_METHOD(void, store_uint8_t_result, (uint8_t), ());
    MOCK_METHOD(void, store_uint16_t_result, (uint16_t), ());
    MOCK_METHOD(void, store_uint32_t_result, (uint32_t), ());
    MOCK_METHOD(void, store_uint64_t_result, (uint64_t), ());
    MOCK_METHOD(void, store_float_result, (float), ());
    MOCK_METHOD(void, store_double_result, (double), ());
    MOCK_METHOD(void, store_long_double_result, (long double), ());
    MOCK_METHOD(void, store_string_result, (const std::string&), ());
    MOCK_METHOD(void,
                store_object_result,
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

    virtual void exception_thrown() override { exception_thrown_noargs(); }
};
