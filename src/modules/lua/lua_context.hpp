// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/core/component.hpp"
#include "mge/script/script_context.hpp"

#include <memory>
#include <vector>

namespace mge::lua {

    class lua_engine;

    /**
     * @brief A Lua script context.
     *
     * A Lua script context represents a single execution context for Lua
     * scripts.
     */
    class lua_context : public mge::script_context
    {
    public:
        lua_context(lua_engine* engine);
        ~lua_context();
        void eval(const std::string& code) override;
        int  main(int argc, const char** argv) override;
        void bind() override;

        lua_State* lua_state() const
        {
            return m_lua_state;
        }

    private:
        void create_helper_module();
        void register_class_function();
        void register_component_function();

        static int component_call(lua_State* L);
        static int create_component_call(lua_State* L);

        lua_engine* m_engine;
        lua_State*  m_lua_state;
        std::vector<std::unique_ptr<mge::dynamic_implementation_registry_entry>>
            m_component_entries;
    };

} // namespace mge::lua