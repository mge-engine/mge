// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/callable.hpp"
#include "mge/core/function_traits.hpp"
#include "mge/core/nth_type.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <array>
#include <functional>
#include <type_traits>
#include <vector>

namespace mge::script {

    class MGESCRIPT_EXPORT function_base
    {
    public:
        function_base() = default;
        virtual ~function_base() = default;
        function_base(const function_base&) = default;
        function_base(function_base&&) = default;

        const std::string&                  name() const;
        const mge::script::invoke_function& invoke_function() const;
        const std::type_index&              return_type() const;
        const std::vector<std::type_index>& argument_types() const;
        const function_details_ref&         details() const;
        function_details_ref&               details();

    protected:
        template <size_t N>
        function_details_ref
        create_details(const std::string&                    name,
                       void*                                 fptr,
                       const mge::script::invoke_function&   function,
                       const std::type_index&                return_type,
                       const std::array<std::type_index, N>& argument_types)
        {
            std::vector<std::type_index> argument_types_v(
                argument_types.begin(),
                argument_types.end());

            return create_details(name,
                                  fptr,
                                  function,
                                  return_type,
                                  std::move(argument_types_v));
        }

        function_details_ref
        create_details(const std::string&                  name,
                       void*                               fptr,
                       const mge::script::invoke_function& function,
                       const std::type_index&              return_type);

        function_details_ref
        create_details(const std::string&                  name,
                       void*                               fptr,
                       const mge::script::invoke_function& function,
                       const std::type_index&              return_type,
                       std::vector<std::type_index>&&      argument_types);

        function_details_ref m_details;
    };

    template <typename R, typename... Args>
    class c_function : public function_base
    {
    private:
        template <typename... InvokeArgs> struct invoke_helper
        {
            template <typename InvokeResult, std::size_t... I>
            static inline void
            call_cfunction(InvokeResult (*fptr)(InvokeArgs...),
                           call_context& context,
                           std::index_sequence<I...>)
            {
                if constexpr (std::is_void_v<InvokeResult>) {
                    (*fptr)(
                        context.parameter<nth_type<I, InvokeArgs...>>(I)...);
                } else {
                    context.store_result((*fptr)(
                        context.parameter<nth_type<I, InvokeArgs...>>(I)...));
                }
            }
        };

    public:
        c_function(const std::string& name, R (*fptr)(Args... args))
        {
            if constexpr (sizeof...(Args) >= 1) {
                auto invoke_function = [fptr](call_context& context) {
                    invoke_helper<Args...>::call_cfunction(
                        fptr,
                        context,
                        std::make_index_sequence<sizeof...(Args)>{});
                };
                std::array<std::type_index, sizeof...(Args)> arg_types = {
                    std::type_index(typeid(Args))...};
                auto result_type = std::type_index(typeid(R));
                m_details = create_details(name,
                                           fptr,
                                           invoke_function,
                                           result_type,
                                           arg_types);
            } else {
                auto invoke_function = [fptr](call_context& context) {
                    invoke_helper<Args...>::call_cfunction(
                        fptr,
                        context,
                        std::make_index_sequence<sizeof...(Args)>{});
                };
                auto result_type = std::type_index(typeid(R));
                m_details =
                    create_details(name, fptr, invoke_function, result_type);
            }
        }

        c_function(const c_function& f) = default;
        c_function(c_function&& f) = default;
    };

    template <typename R, typename... Args>
    inline c_function<R, Args...> function(const std::string& name,
                                           R (*fptr)(Args...))
    {
        return c_function<R, Args...>(name, fptr);
    }

    template <typename R, typename... Args>
    class std_function : public function_base
    {
    private:
        template <typename... InvokeArgs> struct invoke_helper
        {
            template <typename InvokeResult, std::size_t... I>
            static inline void call_stdfunction(
                const std::function<InvokeResult(InvokeArgs...)>& f,
                call_context&                                     context,
                std::index_sequence<I...>)
            {
                if constexpr (std::is_void_v<InvokeResult>) {
                    f(context.parameter<nth_type<I, InvokeArgs...>>(I)...);
                } else {
                    context.store_result(
                        f(context.parameter<nth_type<I, InvokeArgs...>>(I)...));
                }
            }
        };

    public:
        std_function(const std::string&               name,
                     const std::function<R(Args...)>& f)
        {
            if constexpr (sizeof...(Args) >= 1) {
                auto invoke_function = [f](call_context& context) {
                    invoke_helper<Args...>::call_stdfunction(
                        f,
                        context,
                        std::make_index_sequence<sizeof...(Args)>{});
                };
                std::array<std::type_index, sizeof...(Args)> arg_types = {
                    std::type_index(typeid(Args))...};
                auto result_type = std::type_index(typeid(R));
                m_details = create_details(name,
                                           nullptr,
                                           invoke_function,
                                           result_type,
                                           arg_types);
            } else {
                auto invoke_function = [f](call_context& context) {
                    invoke_helper<Args...>::call_stdfunction(
                        f,
                        context,
                        std::make_index_sequence<sizeof...(Args)>{});
                };
                auto result_type = std::type_index(typeid(R));
                m_details =
                    create_details(name, nullptr, invoke_function, result_type);
            }
        }

        std_function(const std_function& f) = default;
        std_function(std_function&& f) = default;
    };

    template <typename R, typename... Args>
    inline std_function<R, Args...> function(const std::string& name,
                                             const std::function<R(Args...)>& f)
    {
        return std_function<R, Args...>(name, f);
    }

    template <typename C> class callable_function : public function_base
    {
        template <typename T> struct callable_utils
        {};

        template <typename R, typename... Args>
        struct callable_utils<R (C::*)(Args...) const>
        {
            template <typename... InvokeArgs> struct invoke_helper
            {
                template <std::size_t... I>
                static inline void call_callable(const C&      f,
                                                 call_context& context,
                                                 std::index_sequence<I...>)
                {
                    if constexpr (std::is_void_v<R>) {
                        f(context.parameter<nth_type<I, InvokeArgs...>>(I)...);
                    } else {
                        context.store_result(
                            f(context.parameter<nth_type<I, InvokeArgs...>>(
                                I)...));
                    }
                }
            };

            using return_type = R;

            static constexpr auto arity = sizeof...(Args);

            static std::array<std::type_index, sizeof...(Args)> arg_types()
            {
                std::array<std::type_index, sizeof...(Args)> types = {
                    std::type_index(typeid(Args))...};
                return types;
            }

            static void invoke(const C& callable, call_context& context)
            {
                invoke_helper<Args...>::call_callable(
                    callable,
                    context,
                    std::make_index_sequence<sizeof...(Args)>{});
            }
        };

        using concrete_utils = callable_utils<decltype(&C::operator())>;

    public:
        callable_function(const std::string& name, C&& callable)
        {
            if constexpr (concrete_utils::arity >= 1) {
                auto invoke_function =
                    [c = std::move(callable)](call_context& context) {
                        concrete_utils::invoke(c, context);
                    };
                auto arg_types = concrete_utils::arg_types();
                auto result_type =
                    std::type_index(typeid(concrete_utils::return_type));
                m_details = create_details(name,
                                           nullptr,
                                           invoke_function,
                                           result_type,
                                           arg_types);
            } else {
                auto result_type =
                    std::type_index(typeid(concrete_utils::return_type));
                auto invoke_function =
                    [c = std::move(callable)](call_context& context) {
                        concrete_utils::invoke(c, context);
                    };
                m_details =
                    create_details(name, nullptr, invoke_function, result_type);
            }
        }

        callable_function(const callable_function& f) = default;
        callable_function(callable_function&& f) = default;
    };

    template <typename C>
    inline std::enable_if<mge::is_callable<C>::value,
                          callable_function<C>>::type
    function(const std::string& name, C&& callable)
    {
        return callable_function<C>(name, std::move(callable));
    }

} // namespace mge::script