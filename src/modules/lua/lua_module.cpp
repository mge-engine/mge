// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_module.hpp"

namespace mge::lua {

    lua_module::lua_module(const mge::reflection::module_details& details)
        : m_details(&details)
    {}

    lua_module::~lua_module() {}

} // namespace mge::lua