#include "test/googlemock.hpp"
#include "modules/directx12/error.hpp"
#include "mge/core/exception.hpp"
namespace dx12 {

    TEST(error, check_hresult_S_OK_has_no_effect)
    {
        error::check_hresult(S_OK, __FILE__, __LINE__, "clazz", "method");
    }

    TEST(error, check_hresult_throws_correct_exception)
    {
        try {
            error::check_hresult(E_OUTOFMEMORY, __FILE__, __LINE__, "clazz", "method");
        } catch(const mge::exception& ex) {
            auto msg = ex.get<mge::exception::message>();
            EXPECT_EQ(std::string("Call to clazz::method failed: 8007000e"), msg);
        }
    }

}
