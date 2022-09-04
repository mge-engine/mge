// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_details.hpp"

#include "lua.hpp"
#include "lua_context.hpp"

namespace mge::lua {
    MGE_DECLARE_REF(lua_type);

    class lua_type
    {
        lua_type(lua_context&                         context,
                 const mge::script::type_details_ref& type);
        ~lua_type();
        const mge::script::type_details& details() const { return *m_type; }

    private:
        const mge::script::type_details_ref& m_type;
    };
} // namespace mge::lua