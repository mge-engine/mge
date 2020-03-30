// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"
#include "modules/directx11/error.hpp"
#include "test/googlemock.hpp"
namespace dx11 {

    TEST(error, check_hresult_S_OK_has_no_effect)
    {
        error::check_hresult(S_OK, __FILE__, __LINE__, "clazz", "method");
    }

    TEST(error, check_hresult_throws_correct_exception)
    {
        try {
            error::check_hresult(E_OUTOFMEMORY, __FILE__, __LINE__, "clazz",
                                 "method");
        } catch (const mge::exception &ex) {
            auto msg = ex.get<mge::exception::message>();
            EXPECT_EQ(std::string("Call to clazz::method failed: 8007000e"),
                      msg);
        }
    }

} // namespace dx11
