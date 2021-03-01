// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/system_error.hpp"

namespace mge {
    system_error::system_error() { set_info(error_code(GetLastError())); }

    system_error::system_error(system_error::error_code_type e)
    {
        set_info(error_code(e));
    }

    system_error::system_error(const system_error &e) : exception(e) {}

    system_error::system_error(system_error &&e) : exception(std::move(e)) {}

    system_error::~system_error() {}

    system_error &system_error::operator=(const system_error &e)
    {
        exception::operator=(e);
        return *this;
    }
} // namespace mge