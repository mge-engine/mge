// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
namespace dx12 {
    MGE_DEFINE_EXCEPTION(error);

    void
    error::check_hresult(HRESULT rc, const char *file, int line, const char *clazz, const char *method)
    {
        if(rc == S_OK) {
            return;
        } else {
            std::string called_function(clazz);
            called_function.append("::");
            called_function.append(method);
            throw ::dx12::error()
                .set_info(mge::exception::source_file(file))
                .set_info(mge::exception::source_line(line))
                .set_info(mge::exception::called_function(called_function.c_str()))
                .set_info(mge::exception::stack(mge::stacktrace()))
                .set_info(mge::exception::type_name(mge::type_name<dx12::error>()))
                << "Call to " << called_function << " failed: " << std::hex << rc;
        }
    }

}
