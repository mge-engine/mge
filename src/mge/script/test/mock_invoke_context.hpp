#include "mge/script/invoke_context.hpp"
#include "test/googlemock.hpp"

class MOCK_invoke_context : public mge::script::invoke_context
{
public:
    MOCK_invoke_context() = default;

    MOCK_METHOD(void, call, (), (override));
};