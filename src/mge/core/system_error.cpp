// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/system_error.hpp"

namespace mge {
    system_error::system_error() { set_error_code(GetLastError()); }

    void system_error::set_error_code(system_error::error_code_type ec)
    {
        set_info(error_code(ec));
#ifdef MGE_OS_WINDOWS
        char* msgbuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                          FORMAT_MESSAGE_FROM_SYSTEM,
                      nullptr,
                      ec,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      (LPTSTR)&msgbuf,
                      0,
                      nullptr);
        try {
            (*this) << "(" << std::hex << ec << std::dec << "): " << msgbuf;
        } catch (...) {
            LocalFree(msgbuf);
            throw;
        }
        LocalFree(msgbuf);
#else
#    error Missing port
#endif
    }

    system_error::system_error(system_error::error_code_type code)
    {
        set_error_code(code);
    }

    system_error::system_error(const system_error& e)
        : exception(e)
    {}

    system_error::system_error(system_error&& e)
        : exception(std::move(e))
    {}

    system_error::~system_error() {}

    system_error& system_error::operator=(const system_error& e)
    {
        exception::operator=(e);
        return *this;
    }
} // namespace mge