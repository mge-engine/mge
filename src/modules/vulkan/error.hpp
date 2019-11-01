#pragma once
// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
namespace vulkan {
    class error : public mge::exception
    {
    public:
        error();
        error(const error& e);
        error(error&& e);
        ~error();

        error& operator=(const error& e);

        template <typename Info>
        error& set_info(const Info& info)
        {
            mge::exception::set_info (info);
            return *this;
        }

        template <typename T>
        error& operator << (const T& value)
        {
            mge::exception::operator <<(value);
            return *this;
        }
    };
}

