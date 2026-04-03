// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/core/trace.hpp"
#include "mge/reflection/invocation_context.hpp"

#include <string>
#include <variant>
#include <vector>

namespace mge::lua {

    /**
     * @brief Lua implementation of the invocation context.
     *
     * Bridges C++ proxy method calls to Lua method implementations.
     * Holds a lua_State and registry references to the Lua class table
     * and self userdata, enabling dispatch of C++ virtual calls into Lua.
     */
    class lua_invocation_context : public mge::reflection::invocation_context
    {
    public:
        /**
         * @brief Construct a Lua invocation context.
         * @param L           Lua state
         * @param class_ref   registry reference to the Lua class table
         * @param self_ref    registry reference to the Lua self userdata
         */
        lua_invocation_context(lua_State* L, int class_ref, int self_ref);
        ~lua_invocation_context() override;

        lua_invocation_context(const lua_invocation_context&) = delete;
        lua_invocation_context&
        operator=(const lua_invocation_context&) = delete;

        bool call_implemented(const char* method) override;

        /**
         * @brief Replace the self reference with a new registry ref
         * to a weak table containing the userdata.
         * The weak table prevents circular references.
         */
        void set_self_ref(int new_self_ref);

    protected:
        void store_bool_argument(size_t index, bool value) override;
        void store_int8_t_argument(size_t index, int8_t value) override;
        void store_uint8_t_argument(size_t index, uint8_t value) override;
        void store_int16_t_argument(size_t index, int16_t value) override;
        void store_uint16_t_argument(size_t index, uint16_t value) override;
        void store_int32_t_argument(size_t index, int32_t value) override;
        void store_uint32_t_argument(size_t index, uint32_t value) override;
        void store_int64_t_argument(size_t index, int64_t value) override;
        void store_uint64_t_argument(size_t index, uint64_t value) override;
        void store_float_argument(size_t index, float value) override;
        void store_double_argument(size_t index, double value) override;
        void store_long_double_argument(size_t      index,
                                        long double value) override;
        void store_string_argument(size_t             index,
                                   const std::string& value) override;

        call_result_type call_method(const char* method) override;

        bool        get_bool_result() override;
        int8_t      get_int8_t_result() override;
        uint8_t     get_uint8_t_result() override;
        int16_t     get_int16_t_result() override;
        uint16_t    get_uint16_t_result() override;
        int32_t     get_int32_t_result() override;
        uint32_t    get_uint32_t_result() override;
        int64_t     get_int64_t_result() override;
        uint64_t    get_uint64_t_result() override;
        float       get_float_result() override;
        double      get_double_result() override;
        long double get_long_double_result() override;
        std::string get_string_result() override;

    private:
        using lua_value = std::
            variant<std::monostate, bool, lua_Integer, lua_Number, std::string>;

        void push_value(const lua_value& v);
        void ensure_argument_slot(size_t index);

        lua_State*             m_lua_state;
        int                    m_class_ref;
        int                    m_self_ref;
        std::vector<lua_value> m_arguments;
        lua_value              m_result;
    };

} // namespace mge::lua
