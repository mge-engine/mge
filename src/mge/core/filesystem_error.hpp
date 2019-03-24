// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/exception.hpp"

namespace mge {

    /**
     * @brief File system error exception
     *
     * Thrown if a file system access failed.
     */
    class MGE_CORE_EXPORT filesystem_error : public ::mge::exception
    {
    public:
        filesystem_error();
        filesystem_error(const filesystem_error& e);
        filesystem_error(filesystem_error&& e);
        ~filesystem_error();

        filesystem_error& operator=(const filesystem_error& e);

        template <typename Info>
        filesystem_error& set_info(const Info& info)
        {
            mge::exception::set_info (info);
            return *this;
        }

        template <typename T>
        filesystem_error& operator << (const T& value)
        {
            mge::exception::operator <<(value);
            return *this;
        }

    };
}
