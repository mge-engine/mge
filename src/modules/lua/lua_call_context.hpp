// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "lua.hpp"
#include "mge/reflection/call_context.hpp"

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

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

        void* callable_parameter(size_t                 index,
                                 const std::type_index& callable_type) override;

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
        void shared_ptr_result(std::shared_ptr<void> value) override;
        void
        primitive_vector_result(const void*            data,
                                size_t                 count,
                                const std::type_index& element_type) override;
        void primitive_vector_parameter(
            size_t                 index,
            void*                  out_vector,
            const std::type_index& element_type) override;

        void exception_thrown(const mge::exception& ex) override;
        void exception_thrown(const std::exception& ex) override;
        void exception_thrown() override;

        int num_results() const
        {
            return m_num_results;
        }

        void set_pointer_result_type(const mge::reflection::type_details* type)
        {
            m_pointer_result_type = type;
        }

        void
        set_shared_ptr_result_type(const mge::reflection::type_details* type)
        {
            m_shared_ptr_result_type = type;
        }

        using callable_factory_fn =
            std::function<std::shared_ptr<void>(lua_State*, int)>;

        static std::unordered_map<std::type_index, callable_factory_fn>&
        callable_factories();

        template <typename R, typename... Args>
        static void register_callable_type();

    private:
        int stack_index(size_t index) const;

        lua_State*                           m_lua_state;
        int                                  m_param_start;
        void*                                m_this_ptr;
        int                                  m_num_results;
        const mge::reflection::type_details* m_pointer_result_type{nullptr};
        const mge::reflection::type_details* m_shared_ptr_result_type{nullptr};
        std::vector<std::shared_ptr<void>>   m_callable_storage;
    };

    namespace detail {

        /**
         * Push a C++ value onto the Lua stack.
         * Used by generic callback wrappers to pass C++ callback
         * arguments to Lua functions.
         */
        template <typename T> inline void push_lua_value(lua_State* L, T&& val)
        {
            using bare =
                std::remove_cv_t<std::remove_reference_t<decltype(val)>>;
            if constexpr (std::is_same_v<bare, bool>) {
                lua_pushboolean(L, val ? 1 : 0);
            } else if constexpr (std::is_integral_v<bare>) {
                lua_pushinteger(L, static_cast<lua_Integer>(val));
            } else if constexpr (std::is_floating_point_v<bare>) {
                lua_pushnumber(L, static_cast<lua_Number>(val));
            } else if constexpr (std::is_enum_v<bare>) {
                lua_pushinteger(
                    L,
                    static_cast<lua_Integer>(
                        static_cast<std::underlying_type_t<bare>>(val)));
            } else {
                lua_pushnil(L);
            }
        }

        /**
         * Read a value from the Lua stack.
         * Used by generic callback wrappers to read Lua return values.
         */
        template <typename T> inline T read_lua_value(lua_State* L, int index)
        {
            using bare = std::remove_cv_t<std::remove_reference_t<T>>;
            if constexpr (std::is_same_v<bare, bool>) {
                return lua_toboolean(L, index) != 0;
            } else if constexpr (std::is_integral_v<bare>) {
                return static_cast<bare>(lua_tointeger(L, index));
            } else if constexpr (std::is_floating_point_v<bare>) {
                return static_cast<bare>(lua_tonumber(L, index));
            } else if constexpr (std::is_enum_v<bare>) {
                return static_cast<bare>(lua_tointeger(L, index));
            } else {
                return bare{};
            }
        }

    } // namespace detail

    /**
     * Ref-counted handle to a Lua function reference.
     * Releases the registry reference when the last copy is destroyed.
     */
    struct lua_callable_ref
    {
        lua_State* L;
        int        ref;

        lua_callable_ref(lua_State* state, int r)
            : L(state)
            , ref(r)
        {}

        ~lua_callable_ref()
        {
            if (ref != LUA_NOREF && ref != LUA_REFNIL) {
                luaL_unref(L, LUA_REGISTRYINDEX, ref);
            }
        }

        lua_callable_ref(const lua_callable_ref&) = delete;
        lua_callable_ref& operator=(const lua_callable_ref&) = delete;
    };

    template <typename R, typename... Args>
    void lua_call_context::register_callable_type()
    {
        callable_factories()[std::type_index(
            typeid(std::function<R(Args...)>))] =
            [](lua_State* L, int stack_idx) -> std::shared_ptr<void> {
            lua_pushvalue(L, stack_idx);
            int  ref = luaL_ref(L, LUA_REGISTRYINDEX);
            auto shared_ref = std::make_shared<lua_callable_ref>(L, ref);

            auto func_ptr = std::make_shared<std::function<R(Args...)>>(
                [L, shared_ref](Args... args) -> R {
                    lua_rawgeti(L, LUA_REGISTRYINDEX, shared_ref->ref);
                    (detail::push_lua_value(L, args), ...);
                    constexpr int nresults = std::is_void_v<R> ? 0 : 1;
                    int rc = lua_pcall(L, sizeof...(Args), nresults, 0);
                    if (rc != 0) {
                        lua_pop(L, 1);
                        if constexpr (!std::is_void_v<R>) {
                            return R{};
                        } else {
                            return;
                        }
                    }
                    if constexpr (!std::is_void_v<R>) {
                        auto result = detail::read_lua_value<R>(L, -1);
                        lua_pop(L, 1);
                        return result;
                    }
                });
            return func_ptr;
        };
    }

} // namespace mge::lua
