// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

namespace mge::dx11 {
    MGE_DEFINE_EXCEPTION_CLASS(error);

    error &error::set_info_from_hresult(HRESULT rc, const char *file, int line,
                                        const char *clazz, const char *method)
    {
        std::string called_function(clazz);
        called_function.append("::");
        called_function.append(method);
        set_info(mge::exception::source_file(file))
                .set_info(mge::exception::source_line(line))
                .set_info(
                    mge::exception::called_function(called_function.c_str()))
                .set_info(mge::exception::stack(mge::stacktrace()))
                .set_info(
                    mge::exception::type_name(mge::type_name<dx11::error>()))
            << "Call to " << called_function << " failed: " << std::hex << rc;
        return *this;
    }

    void error::check_hresult(HRESULT rc, const char *file, int line,
                              const char *clazz, const char *method)
    {
        if (rc == S_OK) {
            return;
        } else {
            dx11::error err;
            throw err.set_info_from_hresult(rc, file, line, clazz, method);
        }
    }

} // namespace mge::dx11