// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/nth_type.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <functional>
#include <iostream>
#include <string>

namespace mge::script {

    namespace details {
        class MGESCRIPT_EXPORT function_base
        {
        public:
            function_base() = default;
            virtual ~function_base() = default;
            const std::string&                        name() const;
            const std::function<void(call_context&)>& invoke() const;
            const function_details_ref& details() const;
            
        protected:
            function_details_ref get_details(void* address);
            function_details_ref
            create_details(const std::string&                   name,
                           void*                                fptr,
                           std::function<void(call_context&)>&& invoker);

            static const std::string
            details_name(const function_details_ref& f);

            static void set_result_type(function_details_ref& f,
                                        std::type_index       type);
            static void add_arg_type(function_details_ref& f,
                                     std::type_index       type);

            function_details_ref m_details;
        };

        template <typename... Args> struct invoke_helper
        {
            template <typename R, std::size_t... I>
            static inline void invoke_function(R (*fptr)(Args...),
                                               call_context& context,
                                               std::index_sequence<I...>)
            {
                context.store_result(
                    (*fptr)(context.parameter<nth_type<I, Args...>>(I)...));
            }
        };

    } // namespace details

    /**
     * @brief Register a callable object (function).
     *
     * @tparam R result type
     * @tparam Args argument types
     */
    template <typename R, typename... Args>
    class function : public details::function_base
    {
    public:
        /**
         * @brief Register a callable object using function pointer.
         *
         * @param name function name
         * @param fptr function pointer
         */
        function(const std::string& name, R (*fptr)(Args... args))
        {
            m_details = get_details(fptr);
            if (m_details) {
                if (details_name(m_details) != name) {
                    MGE_THROW(illegal_state)
                        << "Function '" << name
                        << "' already registered as function '"
                        << details_name(m_details);
                }
            } else {
                if constexpr (sizeof...(Args) >= 1) {
                    auto invoker = [fptr](call_context& context) {
                        details::invoke_helper<Args...>::invoke_function(
                            fptr,
                            context,
                            std::make_index_sequence<sizeof...(Args)>{});
                    };
                    std::array<std::type_index, sizeof...(Args)> arg_types = {
                        std::type_index(typeid(Args))...};
                    auto result_type = std::type_index(typeid(R));
                    m_details = create_details(name, fptr, invoker);
                    set_result_type(m_details, result_type);
                    for (const auto& arg_type : arg_types) {
                        add_arg_type(m_details, arg_type);
                    }
                } else {
                    auto invoker = [fptr](call_context& context) {
                        details::invoke_helper<Args...>::invoke_function(
                            fptr,
                            context,
                            std::make_index_sequence<sizeof...(Args)>{});
                    };
                    auto result_type = std::type_index(typeid(R));
                    m_details = create_details(name, fptr, invoker);
                    set_result_type(m_details, result_type);
                }
            }
        }

        using function_base::invoke;
        using function_base::name;
    };

} // namespace mge::script