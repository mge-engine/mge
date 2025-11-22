// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/call_context.hpp"
#include "test/googlemock.hpp"

namespace mge::reflection {

    class MOCK_call_context : public call_context
    {
    public:
        MOCK_call_context() = default;
        virtual ~MOCK_call_context() = default;
        MOCK_METHOD(void*, this_ptr, (), (override));
        MOCK_METHOD(void, bool_result, (bool value), (override));
        MOCK_METHOD(void, int8_t_result, (int8_t value), (override));
        MOCK_METHOD(void, uint8_t_result, (uint8_t value), (override));
        MOCK_METHOD(void, int16_t_result, (int16_t value), (override));
        MOCK_METHOD(void, uint16_t_result, (uint16_t value), (override));
        MOCK_METHOD(void, int32_t_result, (int32_t value), (override));
        MOCK_METHOD(void, uint32_t_result, (uint32_t value), (override));
        MOCK_METHOD(void, int64_t_result, (int64_t value), (override));
        MOCK_METHOD(void, uint64_t_result, (uint64_t value), (override));
        MOCK_METHOD(void, float_result, (float value), (override));
        MOCK_METHOD(void, double_result, (double value), (override));
        MOCK_METHOD(void, exception_thrown, (const mge::exception& ex), (override));
        MOCK_METHOD(void, exception_thrown, (const std::exception& ex), (override));
        MOCK_METHOD(void, exception_thrown, (), (override));
    };
} // namespace mge::reflection