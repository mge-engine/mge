// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/exception.hpp"

namespace mge {

    /**
     * @brief I/O error.
     *
     * Thrown on a generic input/output error
     * happened.
     */
    class MGE_CORE_EXPORT io_error : public ::mge::exception
    {
    public:
        io_error();
        io_error(const io_error& e);
        io_error(io_error&& e);
        ~io_error();

        io_error& operator=(const io_error& e);

        template <typename Info>
        io_error& operator <<(const Info& info)
        {
            mge::exception::operator << (info);
            return *this;
        }

    };
}
