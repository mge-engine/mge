// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/exception.hpp"
#include "mge/core/types.hpp"

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
        io_error(const io_error &e);
        io_error(io_error &&e);
        ~io_error();

        io_error &operator=(const io_error &e);

        template <typename Info> io_error &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> io_error &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
            return *this;
        }
    };
} // namespace mge
