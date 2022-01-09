// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
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
            std::vector<std::type_index> argument_types_v(argument_types);

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
            template <typename R, std::size_t... I>
            static inline void call_cfunction(R (*fptr)(Args...),
                                              call_context& context,
                                              std::index_sequence<I...>)
            {
                context.store_result(
                    (*fptr)(context.parameter<nth_type<I, Args...>>(I)...));
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

} // namespace mge::script