// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/reflection/call_context.hpp"

namespace mge::lua {

    class lua_call_context : public mge::reflection::call_context
    {
    public:
        lua_call_context(lua_State* L, int param_start, void* this_ptr);
        ~lua_call_context() override;

        bool             bool_parameter(size_t index) override;
        int8_t           int8_t_parameter(size_t index) override;
        uint8_t          uint8_t_parameter(size_t index) override;
        int16_t          int16_t_parameter(size_t index) override;
        uint16_t         uint16_t_parameter(size_t index) override;
        int32_t          int32_t_parameter(size_t index) override;
        uint32_t         uint32_t_parameter(size_t index) override;
        int64_t          int64_t_parameter(size_t index) override;
        uint64_t         uint64_t_parameter(size_t index) override;
        float            float_parameter(size_t index) override;
        double           double_parameter(size_t index) override;
        long double      long_double_parameter(size_t index) override;
        std::string_view string_view_parameter(size_t index) override;
        void*            pointer_parameter(
            size_t                               index,
            const mge::reflection::type_details& details) override;

        void* this_ptr() override;

        void bool_result(bool value) override;
        void int8_t_result(int8_t value) override;
        void uint8_t_result(uint8_t value) override;
        void int16_t_result(int16_t value) override;
        void uint16_t_result(uint16_t value) override;
        void int32_t_result(int32_t value) override;
        void uint32_t_result(uint32_t value) override;
        void int64_t_result(int64_t value) override;
        void uint64_t_result(uint64_t value) override;
        void float_result(float value) override;
        void double_result(double value) override;
        void long_double_result(long double value) override;
        void string_view_result(std::string_view value) override;
        void pointer_result(void* value) override;

        void exception_thrown(const mge::exception& ex) override;
        void exception_thrown(const std::exception& ex) override;
        void exception_thrown() override;

        int num_results() const
        {
            return m_num_results;
        }

    private:
        int stack_index(size_t index) const;

        lua_State* m_lua_state;
        int        m_param_start;
        void*      m_this_ptr;
        int        m_num_results;
    };

} // namespace mge::lua
