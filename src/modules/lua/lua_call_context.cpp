// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_call_context.hpp"

namespace mge::lua {

    lua_call_context::lua_call_context(lua_State* L,
                                       int        param_start,
                                       void*      this_ptr)
        : m_lua_state(L)
        , m_param_start(param_start)
        , m_this_ptr(this_ptr)
        , m_num_results(0)
    {}

    lua_call_context::~lua_call_context() {}

    int lua_call_context::stack_index(size_t index) const
    {
        return m_param_start + static_cast<int>(index);
    }

    void* lua_call_context::this_ptr()
    {
        return m_this_ptr;
    }

    // --- parameter methods ---

    bool lua_call_context::bool_parameter(size_t index)
    {
        return lua_toboolean(m_lua_state, stack_index(index)) != 0;
    }

    int8_t lua_call_context::int8_t_parameter(size_t index)
    {
        return static_cast<int8_t>(
            lua_tointeger(m_lua_state, stack_index(index)));
    }

    uint8_t lua_call_context::uint8_t_parameter(size_t index)
    {
        return static_cast<uint8_t>(
            lua_tointeger(m_lua_state, stack_index(index)));
    }

    int16_t lua_call_context::int16_t_parameter(size_t index)
    {
        return static_cast<int16_t>(
            lua_tointeger(m_lua_state, stack_index(index)));
    }

    uint16_t lua_call_context::uint16_t_parameter(size_t index)
    {
        return static_cast<uint16_t>(
            lua_tointeger(m_lua_state, stack_index(index)));
    }

    int32_t lua_call_context::int32_t_parameter(size_t index)
    {
        return static_cast<int32_t>(
            lua_tointeger(m_lua_state, stack_index(index)));
    }

    uint32_t lua_call_context::uint32_t_parameter(size_t index)
    {
        return static_cast<uint32_t>(
            lua_tointeger(m_lua_state, stack_index(index)));
    }

    int64_t lua_call_context::int64_t_parameter(size_t index)
    {
        return static_cast<int64_t>(
            lua_tointeger(m_lua_state, stack_index(index)));
    }

    uint64_t lua_call_context::uint64_t_parameter(size_t index)
    {
        return static_cast<uint64_t>(
            lua_tointeger(m_lua_state, stack_index(index)));
    }

    float lua_call_context::float_parameter(size_t index)
    {
        return static_cast<float>(
            lua_tonumber(m_lua_state, stack_index(index)));
    }

    double lua_call_context::double_parameter(size_t index)
    {
        return lua_tonumber(m_lua_state, stack_index(index));
    }

    long double lua_call_context::long_double_parameter(size_t index)
    {
        return static_cast<long double>(
            lua_tonumber(m_lua_state, stack_index(index)));
    }

    std::string_view lua_call_context::string_view_parameter(size_t index)
    {
        size_t      len = 0;
        const char* str = lua_tolstring(m_lua_state, stack_index(index), &len);
        return std::string_view(str, len);
    }

    void* lua_call_context::pointer_parameter(
        size_t index, const mge::reflection::type_details& /* details */)
    {
        return lua_touserdata(m_lua_state, stack_index(index));
    }

    // --- result methods ---

    void lua_call_context::bool_result(bool value)
    {
        lua_pushboolean(m_lua_state, value ? 1 : 0);
        ++m_num_results;
    }

    void lua_call_context::int8_t_result(int8_t value)
    {
        lua_pushinteger(m_lua_state, value);
        ++m_num_results;
    }

    void lua_call_context::uint8_t_result(uint8_t value)
    {
        lua_pushinteger(m_lua_state, value);
        ++m_num_results;
    }

    void lua_call_context::int16_t_result(int16_t value)
    {
        lua_pushinteger(m_lua_state, value);
        ++m_num_results;
    }

    void lua_call_context::uint16_t_result(uint16_t value)
    {
        lua_pushinteger(m_lua_state, value);
        ++m_num_results;
    }

    void lua_call_context::int32_t_result(int32_t value)
    {
        lua_pushinteger(m_lua_state, value);
        ++m_num_results;
    }

    void lua_call_context::uint32_t_result(uint32_t value)
    {
        lua_pushinteger(m_lua_state, value);
        ++m_num_results;
    }

    void lua_call_context::int64_t_result(int64_t value)
    {
        lua_pushinteger(m_lua_state, value);
        ++m_num_results;
    }

    void lua_call_context::uint64_t_result(uint64_t value)
    {
        lua_pushinteger(m_lua_state, static_cast<lua_Integer>(value));
        ++m_num_results;
    }

    void lua_call_context::float_result(float value)
    {
        lua_pushnumber(m_lua_state, value);
        ++m_num_results;
    }

    void lua_call_context::double_result(double value)
    {
        lua_pushnumber(m_lua_state, value);
        ++m_num_results;
    }

    void lua_call_context::long_double_result(long double value)
    {
        lua_pushnumber(m_lua_state, static_cast<double>(value));
        ++m_num_results;
    }

    void lua_call_context::string_view_result(std::string_view value)
    {
        lua_pushlstring(m_lua_state, value.data(), value.size());
        ++m_num_results;
    }

    void lua_call_context::pointer_result(void* value)
    {
        lua_pushlightuserdata(m_lua_state, value);
        ++m_num_results;
    }

    // --- exception methods ---

    void lua_call_context::exception_thrown(const mge::exception& ex)
    {
        luaL_error(m_lua_state, "%s", ex.what());
    }

    void lua_call_context::exception_thrown(const std::exception& ex)
    {
        luaL_error(m_lua_state, "%s", ex.what());
    }

    void lua_call_context::exception_thrown()
    {
        luaL_error(m_lua_state, "unknown exception");
    }

} // namespace mge::lua
