// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/core/exception.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge::lua {

    class error : public exception
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

        static void check_status(int status, lua_State* state);
    };

#define CHECK_STATUS(status, state) mge::lua::error::check_status(status, state)

} // namespace mge::lua