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
        , boost::exception(e)
    {}

    exception::exception(exception&& e)
        : std::exception(e)
        , boost::exception(e)
    {}

    exception&
    exception::operator =(const exception& e)
    {
        std::exception::operator =(e);
        boost::exception::operator =(e);
        return *this;
    }

    const char *
    exception::what() const
    {
        auto desc = this->info<excinfo_desc>();
        if (desc) {
            return desc->c_str();
        } else {
            return std::exception::what();
        }
    }

}
