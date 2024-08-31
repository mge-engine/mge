// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/function_data.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type.hpp"

#include <cstdint>
#include <iostream>
#include <tuple>

namespace mge::script {

    namespace {} // namespace

    template <typename R, typename... Args> class function
    {
    public:
        function() {}

        function(const function& f)
            : m_data(f.m_data)
        {}
        function(function&& f) noexcept
            : m_data(std::move(f.m_data))
        {}
        function& operator=(const function& f)
        {
            m_data = f.m_data;
            return *this;
        }
        function& operator=(function&& f) noexcept
        {
            m_data = std::move(f.m_data);
            return *this;
        }

        function(const char* name, R (*f)(Args...))
            : m_data(std::make_shared<function_data>(
                  name, reinterpret_cast<void*>(f)))
        {
            if constexpr (sizeof...(Args) == 0) {
                if constexpr (std::is_same_v<R, void>) {
                    m_data->set_invoker([f](call_context& ctx) {
                        try {
                            f();
                        } catch (const mge::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (const std::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (...) {
                            ctx.exception_thrown();
                        }
                    });
                } else if constexpr (call_context::is_simple_result<R>() ||
                                     std::is_same_v<R, const std::string&>) {
                    m_data->set_invoker([f](call_context& ctx) {
                        try {
                            ctx.store_result(f());
                        } catch (const mge::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (const std::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (...) {
                            ctx.exception_thrown();
                        }
                    });
                } else if constexpr (std::is_reference_v<R> &&
                                     !std::is_const<
                                         std::remove_reference_t<R>>::value) {
                    using base_type = std::remove_reference_t<R>;
                    MGE_THROW_NOT_IMPLEMENTED
                        << "Function with reference to "
                        << type_name<base_type>()
                        << " return value and no arguments";
                } else {
                    MGE_THROW_NOT_IMPLEMENTED
                        << "Function with " << type_name<R>()
                        << " return value and no arguments";
                }
            } else {
                if constexpr (std::is_same_v<R, void>) {
                    m_data->set_invoker([f](call_context& ctx) {
                        try {
                            std::size_t index = 0;
                            f((ctx.get_parameter<Args>(index++))...);
                        } catch (const mge::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (const std::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (...) {
                            ctx.exception_thrown();
                        }
                    });
                } else if constexpr (call_context::is_simple_result<R>() ||
                                     std::is_same_v<R, const std::string&>) {
                    m_data->set_invoker([f](call_context& ctx) {
                        try {
                            std::size_t index = 0;
                            ctx.store_result(
                                f((ctx.get_parameter<Args>(index++))...));
                        } catch (const mge::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (const std::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (...) {
                            ctx.exception_thrown();
                        }
                    });
                } else if constexpr (std::is_enum_v<R>) {
                    m_data->set_invoker([f](call_context& ctx) {
                        try {
                            std::size_t index = 0;
                            ctx.store_result(
                                f((ctx.get_parameter<Args>(index++))...));
                        } catch (const mge::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (const std::exception& e) {
                            ctx.exception_thrown(e);
                        } catch (...) {
                            ctx.exception_thrown();
                        }
                    });
                } else {
                    MGE_THROW_NOT_IMPLEMENTED << "Function with "
                                              << type_name<R>()
                                              << " return value and arguments";
                }
            }
        }

        ~function() = default;

        const function_data_ref& data() const noexcept { return m_data; }

    private:
        function_data_ref m_data;
    };

} // namespace mge::script