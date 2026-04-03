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

    void lua_invocation_context::set_self_ref(int new_self_ref)
    {
        if (m_self_ref != LUA_NOREF) {
            luaL_unref(m_lua_state, LUA_REGISTRYINDEX, m_self_ref);
        }
        m_self_ref = new_self_ref;
    }

    void lua_invocation_context::push_self(lua_State* L)
    {
        if (m_self_ref == LUA_NOREF) {
            lua_pushnil(L);
            return;
        }
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_self_ref);
        lua_rawgeti(L, -1, 1);
        lua_remove(L, -2); // remove weak table, keep userdata
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

    void lua_invocation_context::push_value(const lua_value& v)
    {
        std::visit(
            [this](const auto& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, std::monostate>) {
                    lua_pushnil(m_lua_state);
                } else if constexpr (std::is_same_v<T, bool>) {
                    lua_pushboolean(m_lua_state, val ? 1 : 0);
                } else if constexpr (std::is_same_v<T, lua_Integer>) {
                    lua_pushinteger(m_lua_state, val);
                } else if constexpr (std::is_same_v<T, lua_Number>) {
                    lua_pushnumber(m_lua_state, val);
                } else if constexpr (std::is_same_v<T, std::string>) {
                    lua_pushlstring(m_lua_state, val.c_str(), val.size());
                }
            },
            v);
    }

    void lua_invocation_context::store_bool_argument(size_t index, bool value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = value;
    }

    void lua_invocation_context::store_int8_t_argument(size_t index,
                                                       int8_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_uint8_t_argument(size_t  index,
                                                        uint8_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_int16_t_argument(size_t  index,
                                                        int16_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_uint16_t_argument(size_t   index,
                                                         uint16_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_int32_t_argument(size_t  index,
                                                        int32_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_uint32_t_argument(size_t   index,
                                                         uint32_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_int64_t_argument(size_t  index,
                                                        int64_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_uint64_t_argument(size_t   index,
                                                         uint64_t value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Integer>(value);
    }

    void lua_invocation_context::store_float_argument(size_t index, float value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Number>(value);
    }

    void lua_invocation_context::store_double_argument(size_t index,
                                                       double value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Number>(value);
    }

    void lua_invocation_context::store_long_double_argument(size_t      index,
                                                            long double value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = static_cast<lua_Number>(value);
    }

    void lua_invocation_context::store_string_argument(size_t             index,
                                                       const std::string& value)
    {
        ensure_argument_slot(index);
        m_arguments[index] = value;
    }

    lua_invocation_context::call_result_type
    lua_invocation_context::call_method(const char* method)
    {
        m_result = std::monostate{};

        // Push the method function from the class table
        lua_rawgeti(m_lua_state, LUA_REGISTRYINDEX, m_class_ref);
        lua_getfield(m_lua_state, -1, method);
        lua_remove(m_lua_state, -2); // remove class table, keep function

        if (!lua_isfunction(m_lua_state, -1)) {
            lua_pop(m_lua_state, 1);
            m_arguments.clear();
            return call_result_type::CALL_NOT_FOUND;
        }

        // Push self as first argument (stored in weak table at index 1)
        lua_rawgeti(m_lua_state, LUA_REGISTRYINDEX, m_self_ref);
        lua_rawgeti(m_lua_state, -1, 1);
        lua_remove(m_lua_state, -2); // remove weak table, keep userdata

        // Push stored arguments
        int nargs = 1; // self
        for (const auto& arg : m_arguments) {
            push_value(arg);
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
        if (lua_isboolean(m_lua_state, -1)) {
            m_result = lua_toboolean(m_lua_state, -1) != 0;
        } else if (lua_isnumber(m_lua_state, -1)) {
            m_result = lua_tonumber(m_lua_state, -1);
        } else if (lua_isstring(m_lua_state, -1)) {
            size_t      len = 0;
            const char* s = lua_tolstring(m_lua_state, -1, &len);
            m_result = std::string(s, len);
        }
        lua_pop(m_lua_state, 1); // pop result

        return call_result_type::CALL_EXECUTED;
    }

    bool lua_invocation_context::get_bool_result()
    {
        if (auto* v = std::get_if<bool>(&m_result)) {
            return *v;
        }
        if (auto* v = std::get_if<lua_Integer>(&m_result)) {
            return *v != 0;
        }
        if (auto* v = std::get_if<lua_Number>(&m_result)) {
            return *v != 0.0;
        }
        return false;
    }

    int8_t lua_invocation_context::get_int8_t_result()
    {
        return static_cast<int8_t>(get_int64_t_result());
    }

    uint8_t lua_invocation_context::get_uint8_t_result()
    {
        return static_cast<uint8_t>(get_int64_t_result());
    }

    int16_t lua_invocation_context::get_int16_t_result()
    {
        return static_cast<int16_t>(get_int64_t_result());
    }

    uint16_t lua_invocation_context::get_uint16_t_result()
    {
        return static_cast<uint16_t>(get_int64_t_result());
    }

    int32_t lua_invocation_context::get_int32_t_result()
    {
        return static_cast<int32_t>(get_int64_t_result());
    }

    uint32_t lua_invocation_context::get_uint32_t_result()
    {
        return static_cast<uint32_t>(get_int64_t_result());
    }

    int64_t lua_invocation_context::get_int64_t_result()
    {
        if (auto* v = std::get_if<lua_Integer>(&m_result)) {
            return static_cast<int64_t>(*v);
        }
        if (auto* v = std::get_if<lua_Number>(&m_result)) {
            return static_cast<int64_t>(*v);
        }
        if (auto* v = std::get_if<bool>(&m_result)) {
            return *v ? 1 : 0;
        }
        return 0;
    }

    uint64_t lua_invocation_context::get_uint64_t_result()
    {
        return static_cast<uint64_t>(get_int64_t_result());
    }

    float lua_invocation_context::get_float_result()
    {
        return static_cast<float>(get_double_result());
    }

    double lua_invocation_context::get_double_result()
    {
        if (auto* v = std::get_if<lua_Number>(&m_result)) {
            return static_cast<double>(*v);
        }
        if (auto* v = std::get_if<lua_Integer>(&m_result)) {
            return static_cast<double>(*v);
        }
        if (auto* v = std::get_if<bool>(&m_result)) {
            return *v ? 1.0 : 0.0;
        }
        return 0.0;
    }

    long double lua_invocation_context::get_long_double_result()
    {
        return static_cast<long double>(get_double_result());
    }

    std::string lua_invocation_context::get_string_result()
    {
        if (auto* v = std::get_if<std::string>(&m_result)) {
            return *v;
        }
        return {};
    }

} // namespace mge::lua
