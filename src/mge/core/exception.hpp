// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <boost/exception/all.hpp>
#include "mge/core/dllexport.hpp"

namespace mge {

    /**
     * Base exception class.
     */
    class MGE_CORE_EXPORT exception : public boost::exception
    {
    public:
        exception();
        exception(const exception& ex);
        exception(const boost::exception& ex);
        virtual ~exception();
    };

}
