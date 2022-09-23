// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "lua_fwd.hpp"
#include "mge/script/call_context.hpp"

namespace mge::lua {
    class lua_object_call_context : public mge::script::call_context
    {
    public:
        lua_object_call_context(lua::type* t, lua_State* L, void* userdata)
            : m_type(t)
            , m_lua_state(L)
            , m_userdata(userdata)
        {}
        ~lua_object_call_context() = default;

        void* this_ptr() override;
        void* shared_ptr_address() override;

        bool        bool_parameter(size_t position) override;
        int8_t      int8_t_parameter(size_t position) override;
        uint8_t     uint8_t_parameter(size_t position) override;
        int16_t     int16_t_parameter(size_t position) override;
        uint16_t    uint16_t_parameter(size_t position) override;
        int32_t     int32_t_parameter(size_t position) override;
        uint32_t    uint32_t_parameter(size_t position) override;
        int64_t     int64_t_parameter(size_t position) override;
        uint64_t    uint64_t_parameter(size_t position) override;
        float       float_parameter(size_t position) override;
        double      double_parameter(size_t position) override;
        std::string string_parameter(size_t position) override;

        void* object_parameter(size_t position, std::type_index ti) override;

        void store_bool_result(bool result) override;
        void store_int8_t_result(int8_t result) override;
        void store_uint8_t_result(uint8_t result) override;
        void store_int16_t_result(int16_t result) override;
        void store_uint16_t_result(uint16_t result) override;
        void store_int32_t_result(int32_t result) override;
        void store_uint32_t_result(uint32_t result) override;
        void store_int64_t_result(int64_t result) override;
        void store_uint64_t_result(uint64_t result) override;
        void store_float_result(float result) override;
        void store_double_result(double result) override;
        void store_string_result(const std::string& result) override;

    private:
        lua::type* m_type;
        lua_State* m_lua_state;
        void*      m_userdata;
    };
} // namespace mge::lua