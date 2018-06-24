// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <boost/exception/all.hpp>
#include "mge/core/dllexport.hpp"

/**
 * @file mge/core/exception.hpp
 * @brief Exception base class.
 */

namespace mge {

    /**
     * Base exception class.
     */
    class MGE_CORE_EXPORT exception : public boost::exception
    {
    public:
        /**
         * Constructor.
         */
        exception();

        /**
         * Copy constructor.
         * @param ex copied exception
         */
        exception(const exception& ex);
        /**
         * Constructor from base @c boost::exception
         * @param ex copied exception
         */
        exception(const boost::exception& ex);

        /**
         * Destructor.
         */
        virtual ~exception();
    };

}
