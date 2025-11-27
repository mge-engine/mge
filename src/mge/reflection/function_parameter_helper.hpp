// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/call_context.hpp"
#include "mge/reflection/reflection_fwd.hpp"

#include <utility>

namespace mge::reflection {

    template <typename T>
    inline T function_parameter_helper(call_context& ctx, size_t index)
    {
        if constexpr (std::is_pointer_v<T>) {
            return static_cast<T>(
                ctx.pointer_parameter(index, *get_or_create_type_details<T>()));
        } else {
            return ctx.template parameter<T>(index);
        }
    }

    template <typename F, typename... Args, size_t... Is>
    inline void
    function_caller_impl(call_context& ctx, F func, std::index_sequence<Is...>)
    {
        if constexpr (std::is_same_v<void, std::invoke_result_t<F, Args...>>) {
            func(function_parameter_helper<Args>(ctx, Is)...);
        } else {
            ctx.template result<std::invoke_result_t<F, Args...>>(
                func(function_parameter_helper<Args>(ctx, Is)...));
        }
    }

    template <typename F, typename... Args>
    inline void function_caller(call_context& ctx, F func)
    {
        function_caller_impl<F, Args...>(
            ctx,
            func,
            std::make_index_sequence<sizeof...(Args)>{});
    }

    template <typename T, typename F, typename... Args, size_t... Is>
    inline void method_caller_impl(call_context& ctx,
                                   T&            obj,
                                   F             method_ptr,
                                   std::index_sequence<Is...>)
    {
        if constexpr (std::is_same_v<void,
                                     std::invoke_result_t<F, T&, Args...>>) {
            (obj.*method_ptr)(function_parameter_helper<Args>(ctx, Is)...);
        } else {
            ctx.template result<std::invoke_result_t<F, T&, Args...>>(
                (obj.*method_ptr)(function_parameter_helper<Args>(ctx, Is)...));
        }
    }

    template <typename T, typename F, typename... Args>
    inline void method_caller(call_context& ctx, T& obj, F method_ptr)
    {
        method_caller_impl<T, F, Args...>(
            ctx,
            obj,
            method_ptr,
            std::make_index_sequence<sizeof...(Args)>{});
    }

} // namespace mge::reflection
