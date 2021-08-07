// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
#include "opengl.hpp"
namespace mge::opengl {
    class error : public mge::exception
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

        static void
        check_error(const char* file, int line, const char* signature, const char* glFunction);

        static void
        trace_error(const char* file, int line, const char* signature, const char* glFunction);

        static void clear();
    };
} // namespace mge::opengl

#define TRACE_OPENGL_ERROR(function)                                                               \
    opengl::error::log_error(__FILE__, __LINE__, MGE_FUNCTION_SIGNATURE, #function)

#define CHECK_OPENGL_ERROR(function)                                                               \
    opengl::error::check_error(__FILE__, __LINE__, MGE_FUNCTION_SIGNATURE, #function)
