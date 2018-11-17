#pragma once
#include "mge/core/exception.hpp"
#include "lua.hpp"

namespace lua {

    class error : public mge::exception
    {
    public:
        error();
        error(const error &e);
        error(error&& e);
        ~error();

        error &operator=(const error &e);

        template <typename T>
        error &operator<<(const T &t)
        {
            mge::exception::operator<<(t);
            return *this;
        }

        static void check_status(int status, lua_State *state);
    };

#define CHECK_STATUS(status, state) lua::error::check_status(status, state);
}
