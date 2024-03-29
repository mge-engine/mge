// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge::dx11 {
    class error : public mge::exception
    {
    public:
        error();
        error(const error& e);
        error(error&& e);
        ~error() = default;

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

        error& set_info_from_hresult(HRESULT     rc,
                                     const char* file,
                                     int         line,
                                     const char* clazz,
                                     const char* method);

        static void check_hresult(HRESULT     rc,
                                  const char* file,
                                  int         line,
                                  const char* clazz,
                                  const char* method);

        static void check_dxgi_hresult(HRESULT     rc,
                                       const char* file,
                                       int         line,
                                       const char* clazz,
                                       const char* method);
    };
} // namespace mge::dx11

#define CHECK_HRESULT(rc, clazz, method)                                       \
    ::mge::dx11::error::check_hresult(rc, __FILE__, __LINE__, #clazz, #method)

#define CHECK_DXGI_HRESULT(rc, clazz, method)                                  \
    ::mge::dx11::error::check_dxgi_hresult(rc,                                 \
                                           __FILE__,                           \
                                           __LINE__,                           \
                                           #clazz,                             \
                                           #method)
