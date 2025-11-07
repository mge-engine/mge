// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/system_error.hpp"

namespace mge {
    system_error::system_error()
    {
#ifdef MGE_OS_WINDOWS
        set_error_code(GetLastError());
#elif defined(MGE_OS_LINUX)
        set_error_code(errno);
#else
#    error Missing port
#endif
    }

    void system_error::set_error_code(system_error::error_code_type ec)
    {
        set_info(error_code(ec));
#ifdef MGE_OS_WINDOWS
        char* msgbuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                          FORMAT_MESSAGE_FROM_SYSTEM |
                          FORMAT_MESSAGE_IGNORE_INSERTS,
                      nullptr,
                      ec,
                      MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
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
#elif defined(MGE_OS_LINUX)
        (*this) << "(" << ec << "): " << strerror(ec);
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

    void system_error::check_error(const char* file,
                                   int         line,
                                   const char* signature,
                                   const char* function)
    {
#ifdef MGE_OS_WINDOWS
        auto code = GetLastError();
        if (code == NO_ERROR) {
            return;
        }

        throw system_error(code)
            .set_info(mge::exception::source_file(file))
            .set_info(mge::exception::source_line(line))
            .set_info(mge::exception::function(signature))
            .set_info(mge::exception::stack(mge::stacktrace()))
            .set_info(mge::exception::called_function(function));
#elif defined(MGE_OS_LINUX)
        if (errno == 0) {
            return;
        }

        throw system_error(errno)
            .set_info(mge::exception::source_file(file))
            .set_info(mge::exception::source_line(line))
            .set_info(mge::exception::function(signature))
            .set_info(mge::exception::stack(mge::stacktrace()))
            .set_info(mge::exception::called_function(function));
#else
#    error Missing port
#endif
    }

} // namespace mge