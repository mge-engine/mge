// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "lua_error.hpp"

namespace mge::lua {

    /**
     * @brief Scope guard for Lua stack.
     *
     * This class saves the current stack top on construction
     * and restores it on destruction.
     */
    class stack_check_scope
    {
    public:
        /**
         * @brief Create scope guard.
         * @param L Lua state
         */
        stack_check_scope(lua_State* L)
            : m_lua_state(L)
            , m_top(lua_gettop(L))
        {}

        /**
         * @brief Destructor restores stack top.
         */
        ~stack_check_scope() noexcept(false)
        {
            auto current_top = lua_gettop(m_lua_state);
            if (current_top != m_top) {
                MGE_THROW(mge::lua::error) << "Lua stack imbalance: top was "
                                           << m_top << ", now " << current_top;
            }
        }

    private:
        lua_State* m_lua_state;
        int        m_top;
    };

} // namespace mge::lua