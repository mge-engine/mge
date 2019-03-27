// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"

namespace dx11 {

    class error : public ::mge::exception
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

        static void check_hresult(HRESULT rc, const char *file, int line, const char *clazz, const char *method);
    };
}
#define CHECK_HRESULT(rc, clazz, method) ::dx11::error::check_hresult(rc, __FILE__, __LINE__, #clazz, #method)
