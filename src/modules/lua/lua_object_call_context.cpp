// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "lua_object_call_context.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(LUA);
}

namespace mge::lua {

    void* lua_object_call_context::this_ptr()
    {
        // m_userdata --+
        //              v
        //              +-------------------+
        //              | ptr to shared_ptr |
        //              +----------------+--+
        //                        |
        //                        v
        //                  +------------+
        //                  | shared_ptr |
        //                  +------------+
        //                        |
        //                        v
        //               +-----------------+
        //               | object instance |
        //               +-----------------+
        //
        std::shared_ptr<int>** sptr =
            reinterpret_cast<std::shared_ptr<int>**>(m_userdata);
        if (sptr && *sptr) {
            return (*sptr)->get();
        } else {
            return nullptr;
        }
    }

    void* lua_object_call_context::shared_ptr_address() { return m_userdata; }

    bool lua_object_call_context::bool_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return lua_toboolean(m_lua_state, index + 1 + m_offset) != 0;
    }

    int8_t lua_object_call_context::int8_t_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<int8_t>(
            lua_tointeger(m_lua_state, index + 1 + m_offset));
    }

    int16_t lua_object_call_context::int16_t_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<int16_t>(
            lua_tointeger(m_lua_state, index + 1 + m_offset));
    }

    int32_t lua_object_call_context::int32_t_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<int32_t>(
            lua_tointeger(m_lua_state, index + 1 + m_offset));
    }

    int64_t lua_object_call_context::int64_t_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<int64_t>(
            lua_tointeger(m_lua_state, index + 1 + m_offset));
    }

    uint8_t lua_object_call_context::uint8_t_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<uint8_t>(
            lua_tointeger(m_lua_state, index + 1 + m_offset));
    }

    uint16_t lua_object_call_context::uint16_t_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<uint16_t>(
            lua_tointeger(m_lua_state, index + 1 + m_offset));
    }

    uint32_t lua_object_call_context::uint32_t_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<uint32_t>(
            lua_tointeger(m_lua_state, index + 1 + m_offset));
    }

    uint64_t lua_object_call_context::uint64_t_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<uint64_t>(
            lua_tointeger(m_lua_state, index + 1 + m_offset));
    }

    float lua_object_call_context::float_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<float>(
            lua_tonumber(m_lua_state, index + 1 + m_offset));
    }

    double lua_object_call_context::double_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<double>(
            lua_tonumber(m_lua_state, index + 1 + m_offset));
    }

    std::string lua_object_call_context::string_parameter(size_t position)
    {
        int         index = static_cast<int>(position);
        size_t      len = 0;
        const char* str =
            lua_tolstring(m_lua_state, index + 1 + m_offset, &len);
        return std::string(str, str + len);
    }

    void* lua_object_call_context::object_parameter(size_t position,
                                                    std::type_index /*ti*/)
    {
        int   index = static_cast<int>(position);
        void* userdata = lua_touserdata(m_lua_state, index + 1 + m_offset);
        if (userdata) {
            std::shared_ptr<int>* sptr =
                reinterpret_cast<std::shared_ptr<int>*>(userdata);
            if (sptr) {
                return sptr->get();
            }
        }
        return nullptr;
    }

    void lua_object_call_context::store_bool_result(bool result)
    {
        lua_pushboolean(m_lua_state, result ? 1 : 0);
    }

    void lua_object_call_context::store_int8_t_result(int8_t result)
    {
        lua_Integer luaval = checked_cast<lua_Integer>(result);
        lua_pushinteger(m_lua_state, luaval);
    }

    void lua_object_call_context::store_int16_t_result(int16_t result)
    {
        lua_Integer luaval = checked_cast<lua_Integer>(result);
        lua_pushinteger(m_lua_state, luaval);
    }

    void lua_object_call_context::store_int32_t_result(int32_t result)
    {
        lua_Integer luaval = checked_cast<lua_Integer>(result);
        lua_pushinteger(m_lua_state, luaval);
    }

    void lua_object_call_context::store_int64_t_result(int64_t result)
    {
        lua_Integer luaval = checked_cast<lua_Integer>(result);
        lua_pushinteger(m_lua_state, luaval);
    }

    void lua_object_call_context::store_uint8_t_result(uint8_t result)
    {
        lua_Integer luaval = checked_cast<lua_Integer>(result);
        lua_pushinteger(m_lua_state, luaval);
    }

    void lua_object_call_context::store_uint16_t_result(uint16_t result)
    {
        lua_Integer luaval = checked_cast<lua_Integer>(result);
        lua_pushinteger(m_lua_state, luaval);
    }

    void lua_object_call_context::store_uint32_t_result(uint32_t result)
    {
        lua_Integer luaval = checked_cast<lua_Integer>(result);
        lua_pushinteger(m_lua_state, luaval);
    }

    void lua_object_call_context::store_uint64_t_result(uint64_t result)
    {
        lua_Integer luaval = checked_cast<lua_Integer>(result);
        lua_pushinteger(m_lua_state, luaval);
    }

    void lua_object_call_context::store_float_result(float result)
    {
        lua_pushnumber(m_lua_state, result);
    }

    void lua_object_call_context::store_double_result(double result)
    {
        lua_pushnumber(m_lua_state, result);
    }

    void lua_object_call_context::store_string_result(const std::string& result)
    {
        lua_pushlstring(m_lua_state, result.c_str(), result.size());
    }

    void lua_object_call_context::store_shared_ptr_result(
        const std::type_index& type, const void* address)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

} // namespace mge::lua
