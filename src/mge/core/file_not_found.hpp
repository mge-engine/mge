// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/exception.hpp"

namespace mge {

    /**
     * @brief File not found exception
     *
     * Thrown if a file was not found but is required for
     * the requested operation.
     */
    class MGE_CORE_EXPORT file_not_found : public ::mge::exception
    {
    public:
        file_not_found();
        file_not_found(const file_not_found& e);
        file_not_found(file_not_found&& e);
        ~file_not_found();

        file_not_found& operator=(const file_not_found& e);

        template <typename Info>
        file_not_found& operator <<(const Info& info)
        {
            mge::exception::operator << (info);
            return *this;
        }

    };
}
