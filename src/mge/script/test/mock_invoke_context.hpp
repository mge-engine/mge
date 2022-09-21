#include "mge/script/invoke_context.hpp"
#include "test/googlemock.hpp"

class MOCK_invoke_context : public mge::script::invoke_context
{
public:
    MOCK_invoke_context() = default;

    MOCK_METHOD(void, call, (const char*), (override));

    MOCK_METHOD(bool, retrieve_bool, (), (const override));
    MOCK_METHOD(int8_t, retrieve_int8_t, (), (const override));
    MOCK_METHOD(int16_t, retrieve_int16_t, (), (const override));
    MOCK_METHOD(int32_t, retrieve_int32_t, (), (const override));
    MOCK_METHOD(int64_t, retrieve_int64_t, (), (const override));
    MOCK_METHOD(uint8_t, retrieve_uint8_t, (), (const override));
    MOCK_METHOD(uint16_t, retrieve_uint16_t, (), (const override));
    MOCK_METHOD(uint32_t, retrieve_uint32_t, (), (const override));
    MOCK_METHOD(uint64_t, retrieve_uint64_t, (), (const override));
    MOCK_METHOD(float, retrieve_float, (), (const override));
    MOCK_METHOD(double, retrieve_double, (), (const override));
};