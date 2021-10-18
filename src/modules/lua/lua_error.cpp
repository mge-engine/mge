// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "lua_error.hpp"

namespace mge::lua {
    MGE_DEFINE_EXCEPTION_CLASS(error);

    void error::check_status(int status, lua_State* state) { return; }
} // namespace mge::lua