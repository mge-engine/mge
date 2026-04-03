// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_invocation_context.hpp"
#include "lua_error.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {

    lua_invocation_context::lua_invocation_context(lua_State* L,
                                                   int        class_ref,
                                                   int        self_ref)
        : m_lua_state(L)
        , m_class_ref(class_ref)
        , m_self_ref(self_ref)
    {}

    lua_invocation_context::~lua_invocation_context()
    {
        if (m_lua_state) {
            if (m_class_ref != LUA_NOREF) {
                luaL_unref(m_lua_state, LUA_REGISTRYINDEX, m_class_ref);
            }
            if (m_self_ref != LUA_NOREF) {
                luaL_unref(m_lua_state, LUA_REGISTRYINDEX, m_self_ref);
            }
        }
    }

    bool lua_invocation_context::call_implemented(const char* method)
    {
        // Look up method in the class table
        lua_rawgeti(m_lua_state, LUA_REGISTRYINDEX, m_class_ref);
        lua_getfield(m_lua_state, -1, method);
        bool implemented = lua_isfunction(m_lua_state, -1);
        lua_pop(m_lua_state, 2); // pop function (or nil) and class table
        return implemented;
    }

    void lua_invocation_context::ensure_argument_slot(size_t index)
    {
        if (index >= m_arguments.size()) {
            m_arguments.resize(index + 1);
        }
    }

    void lua_invocation_context::store_bool_argument(size_t index, bool value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::BOOL;
        m_arguments[index].bool_val = value;
    }

    void lua_invocation_context::store_int8_t_argument(size_t index,
                                                       int8_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::INTEGER;
        m_arguments[index].int_val = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_uint8_t_argument(size_t  index,
                                                        uint8_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::INTEGER;
        m_arguments[index].int_val = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_int16_t_argument(size_t  index,
                                                        int16_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::INTEGER;
        m_arguments[index].int_val = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_uint16_t_argument(size_t   index,
                                                         uint16_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::INTEGER;
        m_arguments[index].int_val = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_int32_t_argument(size_t  index,
                                                        int32_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::INTEGER;
        m_arguments[index].int_val = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_uint32_t_argument(size_t   index,
                                                         uint32_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::INTEGER;
        m_arguments[index].int_val = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_int64_t_argument(size_t  index,
                                                        int64_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::INTEGER;
        m_arguments[index].int_val = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_uint64_t_argument(size_t   index,
                                                         uint64_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::INTEGER;
        m_arguments[index].int_val = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_float_argument(size_t index, float value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::NUMBER;
        m_arguments[index].num_val = static_cast<lua_Number>(value);
    }

    void lua_invocation_context::store_double_argument(size_t index,
                                                       double value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::NUMBER;
        m_arguments[index].num_val = static_cast<lua_Number>(value);
    }

    void lua_invocation_context::store_long_double_argument(size_t      index,
                                                            long double value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::NUMBER;
        m_arguments[index].num_val = static_cast<lua_Number>(value);
    }

    void lua_invocation_context::store_string_argument(size_t             index,
                                                       const std::string& value)
    {
        ensure_argument_slot(index);
        m_arguments[index].type = stored_argument::arg_type::STRING;
        m_arguments[index].str_val = value;
    }

    lua_invocation_context::call_result_type
    lua_invocation_context::call_method(const char* method)
    {
        m_has_result = false;

        // Push the method function from the class table
        lua_rawgeti(m_lua_state, LUA_REGISTRYINDEX, m_class_ref);
        lua_getfield(m_lua_state, -1, method);
        lua_remove(m_lua_state, -2); // remove class table, keep function

        if (!lua_isfunction(m_lua_state, -1)) {
            lua_pop(m_lua_state, 1);
            m_arguments.clear();
            return call_result_type::CALL_NOT_FOUND;
        }

        // Push self as first argument
        lua_rawgeti(m_lua_state, LUA_REGISTRYINDEX, m_self_ref);

        // Push stored arguments
        int nargs = 1; // self
        for (const auto& arg : m_arguments) {
            switch (arg.type) {
            case stored_argument::arg_type::BOOL:
                lua_pushboolean(m_lua_state, arg.bool_val ? 1 : 0);
                break;
            case stored_argument::arg_type::INTEGER:
                lua_pushinteger(m_lua_state, arg.int_val);
                break;
            case stored_argument::arg_type::NUMBER:
                lua_pushnumber(m_lua_state, arg.num_val);
                break;
            case stored_argument::arg_type::STRING:
                lua_pushlstring(m_lua_state,
                                arg.str_val.c_str(),
                                arg.str_val.size());
                break;
            }
            ++nargs;
        }
        m_arguments.clear();

        // pcall: nargs arguments, 1 result
        int rc = lua_pcall(m_lua_state, nargs, 1, 0);
        if (rc != LUA_OK) {
            const char* err = lua_tostring(m_lua_state, -1);
            MGE_ERROR_TRACE(LUA,
                            "Lua method '{}' call failed: {}",
                            method,
                            err ? err : "(unknown error)");
            lua_pop(m_lua_state, 1);
            return call_result_type::CALL_FAILED;
        }

        // Store the result from stack top for later retrieval
        if (lua_isnil(m_lua_state, -1)) {
            m_has_result = false;
        } else if (lua_isboolean(m_lua_state, -1)) {
            m_has_result = true;
            m_bool_result = lua_toboolean(m_lua_state, -1) != 0;
            m_int_result = m_bool_result ? 1 : 0;
            m_num_result = m_bool_result ? 1.0 : 0.0;
        } else if (lua_isnumber(m_lua_state, -1)) {
            m_has_result = true;
            m_num_result = lua_tonumber(m_lua_state, -1);
            m_int_result = lua_tointeger(m_lua_state, -1);
            m_bool_result = m_int_result != 0;
        } else if (lua_isstring(m_lua_state, -1)) {
            m_has_result = true;
            size_t      len = 0;
            const char* s = lua_tolstring(m_lua_state, -1, &len);
            m_string_result.assign(s, len);
        }
        lua_pop(m_lua_state, 1); // pop result

        return call_result_type::CALL_EXECUTED;
    }

    bool lua_invocation_context::get_bool_result()
    {
        return m_bool_result;
    }

    int8_t lua_invocation_context::get_int8_t_result()
    {
        return static_cast<int8_t>(m_int_result);
    }

    uint8_t lua_invocation_context::get_uint8_t_result()
    {
        return static_cast<uint8_t>(m_int_result);
    }

    int16_t lua_invocation_context::get_int16_t_result()
    {
        return static_cast<int16_t>(m_int_result);
    }

    uint16_t lua_invocation_context::get_uint16_t_result()
    {
        return static_cast<uint16_t>(m_int_result);
    }

    int32_t lua_invocation_context::get_int32_t_result()
    {
        return static_cast<int32_t>(m_int_result);
    }

    uint32_t lua_invocation_context::get_uint32_t_result()
    {
        return static_cast<uint32_t>(m_int_result);
    }

    int64_t lua_invocation_context::get_int64_t_result()
    {
        return static_cast<int64_t>(m_int_result);
    }

    uint64_t lua_invocation_context::get_uint64_t_result()
    {
        return static_cast<uint64_t>(m_int_result);
    }

    float lua_invocation_context::get_float_result()
    {
        return static_cast<float>(m_num_result);
    }

    double lua_invocation_context::get_double_result()
    {
        return static_cast<double>(m_num_result);
    }

    long double lua_invocation_context::get_long_double_result()
    {
        return static_cast<long double>(m_num_result);
    }

    std::string lua_invocation_context::get_string_result()
    {
        return m_string_result;
    }

} // namespace mge::lua
