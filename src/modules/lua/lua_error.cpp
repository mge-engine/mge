#include "lua_error.hpp"

namespace mge::lua {
    MGE_DEFINE_EXCEPTION_CLASS(error);

    void error::check_status(int status, lua_State* state) { return; }
} // namespace mge::lua