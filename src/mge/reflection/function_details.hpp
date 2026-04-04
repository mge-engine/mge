// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/call_context.hpp"
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/function_parameter_helper.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/signature.hpp"
#include "mge/reflection/type.hpp"
#include "mge/reflection/visitor.hpp"

#include <tuple>

namespace mge::reflection {

    class MGEREFLECTION_EXPORT function_details
    {
    public:
        function_details();
        ~function_details();

        template <typename Ret, typename... Args>
        function_details(const char* name, Ret (*func)(Args...))
            : m_signature(make_type_identifier<Ret>(),
                          {make_type_identifier<Args>()...})
            , m_name(name)
            , m_noexcept(false)
        {
            if constexpr (sizeof...(Args) == 0 && std::is_same_v<Ret, void>) {
                m_invoke_function = [func](call_context& ctx) {
                    try {
                        func();
                    } catch (const mge::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (const std::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                };
            } else {
                m_invoke_function = [func](call_context& ctx) {
                    try {
                        function_caller<Ret (*)(Args...), Args...>(ctx, func);
                    } catch (const mge::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (const std::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                };
            }
        }

        template <typename Ret, typename... Args>
        function_details(const char* name, Ret (*func)(Args...) noexcept)
            : m_signature(make_type_identifier<Ret>(),
                          {make_type_identifier<Args>()...})
            , m_name(name)
            , m_noexcept(true)
        {
            if constexpr (sizeof...(Args) == 0 && std::is_same_v<Ret, void>) {
                m_invoke_function = [func](call_context& ctx) { func(); };
            } else {
                m_invoke_function = [func](call_context& ctx) {
                    function_caller<Ret (*)(Args...) noexcept, Args...>(ctx,
                                                                        func);
                };
            }
        }

        template <typename Ret, typename... Args>
        function_details(const char*                        name,
                         const std::function<Ret(Args...)>& func)
            : m_signature(make_type_identifier<Ret>(),
                          {make_type_identifier<Args>()...})
            , m_name(name)
            , m_noexcept(false)
        {
            if constexpr (sizeof...(Args) == 0 && std::is_same_v<Ret, void>) {
                m_invoke_function = [func](call_context& ctx) {
                    try {
                        func();
                    } catch (const mge::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (const std::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                };
            } else {
                m_invoke_function = [func](call_context& ctx) {
                    try {
                        function_caller<decltype(func), Args...>(ctx, func);
                    } catch (const mge::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (const std::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                };
            }
        }

        function_details(const function_details&) = delete;
        function_details& operator=(const function_details&) = delete;
        function_details(function_details&&) = delete;
        function_details& operator=(function_details&&) = delete;

        const mge::reflection::signature& signature() const noexcept
        {
            return m_signature;
        }

        const std::string& name() const noexcept
        {
            return m_name;
        }

        bool is_noexcept() const noexcept
        {
            return m_noexcept;
        }

        void invoke(call_context& ctx) const
        {
            m_invoke_function(ctx);
        }

        module_details_weak_ref module() const noexcept
        {
            return m_module;
        }

        void apply(visitor& v) const;

    private:
        mge::reflection::signature m_signature;
        std::string                m_name;
        invoke_function            m_invoke_function;
        bool                       m_noexcept;
        module_details_weak_ref    m_module;

        friend class module_details;
    };

} // namespace mge::reflection
