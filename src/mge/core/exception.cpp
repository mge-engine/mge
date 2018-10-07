// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"

namespace mge {

    exception::~exception()
    {}

    exception::exception()
    {}

    exception::exception(const exception& e)
        : std::exception(e)
    {}

    exception::exception(exception&& e)
        : std::exception(std::move(e))
    {}

    exception&
    exception::operator =(const exception& e)
    {
        std::exception::operator =(e);
        return *this;
    }

    const char *
    exception::what() const
    {
        return std::exception::what();
    }

}
