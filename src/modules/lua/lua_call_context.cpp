// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_call_context.hpp"
#include "lua_binder.hpp"

#include <string>
#include <string_view>
#include <typeindex>
#include <vector>

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
        void* ud = lua_touserdata(m_lua_state, stack_index(index));
        if (ud && lua_type(m_lua_state, stack_index(index)) == LUA_TUSERDATA) {
            auto* header = static_cast<lua_instance_header*>(ud);
            return lua_binder::instance_object_ptr(header);
        }
        return ud;
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
        if (m_pointer_result_type) {
            lua_binder::create_foreign_instance(m_lua_state,
                                                m_pointer_result_type,
                                                value);
        } else {
            lua_pushlightuserdata(m_lua_state, value);
        }
        ++m_num_results;
    }

    void lua_call_context::shared_ptr_result(std::shared_ptr<void> value)
    {
        if (m_shared_ptr_result_type) {
            lua_binder::create_shared_instance(m_lua_state,
                                               m_shared_ptr_result_type,
                                               std::move(value));
        } else {
            lua_pushlightuserdata(m_lua_state, value.get());
        }
        ++m_num_results;
    }

    // --- primitive vector methods ---

    void lua_call_context::primitive_vector_result(
        const void* data, size_t count, const std::type_index& element_type)
    {
        lua_createtable(m_lua_state, static_cast<int>(count), 0);

        for (size_t i = 0; i < count; ++i) {
            if (element_type == std::type_index(typeid(bool))) {
                lua_pushboolean(m_lua_state,
                                static_cast<const bool*>(data)[i] ? 1 : 0);
            } else if (element_type == std::type_index(typeid(int8_t))) {
                lua_pushinteger(m_lua_state,
                                static_cast<const int8_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(uint8_t))) {
                lua_pushinteger(m_lua_state,
                                static_cast<const uint8_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(int16_t))) {
                lua_pushinteger(m_lua_state,
                                static_cast<const int16_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(uint16_t))) {
                lua_pushinteger(m_lua_state,
                                static_cast<const uint16_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(int32_t))) {
                lua_pushinteger(m_lua_state,
                                static_cast<const int32_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(uint32_t))) {
                lua_pushinteger(m_lua_state,
                                static_cast<const uint32_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(int64_t))) {
                lua_pushinteger(m_lua_state,
                                static_cast<const int64_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(uint64_t))) {
                lua_pushinteger(m_lua_state,
                                static_cast<lua_Integer>(
                                    static_cast<const uint64_t*>(data)[i]));
            } else if (element_type == std::type_index(typeid(float))) {
                lua_pushnumber(m_lua_state, static_cast<const float*>(data)[i]);
            } else if (element_type == std::type_index(typeid(double))) {
                lua_pushnumber(m_lua_state,
                               static_cast<const double*>(data)[i]);
            } else if (element_type == std::type_index(typeid(long double))) {
                lua_pushnumber(m_lua_state,
                               static_cast<double>(
                                   static_cast<const long double*>(data)[i]));
            } else if (element_type == std::type_index(typeid(std::string))) {
                const auto& s = static_cast<const std::string*>(data)[i];
                lua_pushlstring(m_lua_state, s.data(), s.size());
            } else if (element_type ==
                       std::type_index(typeid(std::string_view))) {
                const auto& sv = static_cast<const std::string_view*>(data)[i];
                lua_pushlstring(m_lua_state, sv.data(), sv.size());
            }
            lua_rawseti(m_lua_state, -2, static_cast<int>(i + 1));
        }
        ++m_num_results;
    }

    void lua_call_context::primitive_vector_parameter(
        size_t index, void* out_vector, const std::type_index& element_type)
    {
        int si = stack_index(index);
        int n = static_cast<int>(lua_objlen(m_lua_state, si));

        for (int i = 1; i <= n; ++i) {
            lua_rawgeti(m_lua_state, si, i);

            if (element_type == std::type_index(typeid(bool))) {
                static_cast<std::vector<bool>*>(out_vector)
                    ->push_back(lua_toboolean(m_lua_state, -1) != 0);
            } else if (element_type == std::type_index(typeid(int8_t))) {
                static_cast<std::vector<int8_t>*>(out_vector)
                    ->push_back(
                        static_cast<int8_t>(lua_tointeger(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(uint8_t))) {
                static_cast<std::vector<uint8_t>*>(out_vector)
                    ->push_back(
                        static_cast<uint8_t>(lua_tointeger(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(int16_t))) {
                static_cast<std::vector<int16_t>*>(out_vector)
                    ->push_back(
                        static_cast<int16_t>(lua_tointeger(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(uint16_t))) {
                static_cast<std::vector<uint16_t>*>(out_vector)
                    ->push_back(
                        static_cast<uint16_t>(lua_tointeger(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(int32_t))) {
                static_cast<std::vector<int32_t>*>(out_vector)
                    ->push_back(
                        static_cast<int32_t>(lua_tointeger(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(uint32_t))) {
                static_cast<std::vector<uint32_t>*>(out_vector)
                    ->push_back(
                        static_cast<uint32_t>(lua_tointeger(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(int64_t))) {
                static_cast<std::vector<int64_t>*>(out_vector)
                    ->push_back(
                        static_cast<int64_t>(lua_tointeger(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(uint64_t))) {
                static_cast<std::vector<uint64_t>*>(out_vector)
                    ->push_back(
                        static_cast<uint64_t>(lua_tointeger(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(float))) {
                static_cast<std::vector<float>*>(out_vector)
                    ->push_back(
                        static_cast<float>(lua_tonumber(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(double))) {
                static_cast<std::vector<double>*>(out_vector)
                    ->push_back(lua_tonumber(m_lua_state, -1));
            } else if (element_type == std::type_index(typeid(long double))) {
                static_cast<std::vector<long double>*>(out_vector)
                    ->push_back(static_cast<long double>(
                        lua_tonumber(m_lua_state, -1)));
            } else if (element_type == std::type_index(typeid(std::string))) {
                size_t      len = 0;
                const char* str = lua_tolstring(m_lua_state, -1, &len);
                static_cast<std::vector<std::string>*>(out_vector)
                    ->emplace_back(str, len);
            } else if (element_type ==
                       std::type_index(typeid(std::string_view))) {
                size_t      len = 0;
                const char* str = lua_tolstring(m_lua_state, -1, &len);
                static_cast<std::vector<std::string_view>*>(out_vector)
                    ->emplace_back(str, len);
            }

            lua_pop(m_lua_state, 1);
        }
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
