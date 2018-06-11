// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"

namespace mge {
    exception::exception()
        :boost::exception()
    {}

    exception::exception(const exception& ex)
        :boost::exception(ex)
    {}

    exception::exception(const boost::exception& ex)
        :boost::exception(ex)
    {}

    exception::~exception()
    {}
}
