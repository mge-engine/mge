// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
#include "mge/core/stdexceptions.hpp"
#include "python.hpp"

namespace mge::python {

    class error : public exception
    {
    public:
        error();
        error(const error& e);
        error(error&& e);
        ~error();

        error& operator=(const error& e);

        template <typename Info> error& set_info(const Info& info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> error& operator<<(const T& value)
        {
            mge::exception::operator<<(value);
            return *this;
        }
    };

} // namespace mge::python