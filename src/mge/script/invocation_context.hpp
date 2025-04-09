// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_identifier.hpp"
#include <utility>
namespace mge::script {

    /**
     * @brief Context for implememting methods in a script language.
     *
     * The invocation context is used to implement methods in a script language,
     * i.e. to implememnt C++ interfaces.
     *
     * When a method of a C++ interface is called, the protocol is as follows:
     *
     * 1. It is checked if the method is implemented in the script language
     *    using <tt>call_implemented</tt>. If the method is not implemented, the
     *
     * call is forwarded to the base class.
     * 2. If the method is implemented, the arguments are stored using the
     *    <tt>store_*</tt> methods. Arguments are stored in the order they are
     *    passed to the method.
     * 3. The method is called using <tt>call_method</tt>. The method name is
     *   passed as argument, allowing to dispatch the call.
     * 4. If the call returns <tt>CALL_EXECUTED</tt>, the result is retrieved
     *    using one of the <tt>get_*_result</tt> methods.
     * 5. If the call returns <tt>CALL_NOT_FOUND</tt>, the call is forwarded to
     *   the base class.
     * 6. If the call returns <tt>CALL_FAILED</tt>, an error was raised
     *   the script language and an exception is thrown in C++.
     */
    class MGESCRIPT_EXPORT invocation_context
    {
    public:
        /**
         * @brief Result of a call.
         */
        enum class call_result_type
        {
            CALL_EXECUTED = 0, // call was executed, result is available
            CALL_NOT_FOUND =
                1,          // call was not found, e.g. method does not
                            // exist in script language interface implementation
            CALL_FAILED = 2 // call failed, e.g. exception was thrown
        };

        invocation_context() = default;
        virtual ~invocation_context() = default;

        template <typename Signature> struct call_helper;
        template <typename R, typename... Args> struct call_helper<R(Args...)>
        {
            static R
            call(invocation_context* ctx, const char* method, Args... args)
            {
                return ctx->call<R, Args...>(method,
                                             std::forward<Args>(args)...);
            }
        };

        /**
         * @brief Call a method with arguments.
         * @tparam R return type
         * @tparam Args argument types
         * @param method method name
         * @param args arguments
         * @return result of the method call
         */
        template <typename R, typename... Args>
        R call(const char* method, Args... args)
        {
            size_t index = 0;
            (store_argument<Args>(index++, args), ...);
            return invoke_method<R>(method);
        }

    public:
        template <typename T> void store_argument(size_t index, T value)
        {
            using PlainType = std::remove_cv_t<T>;
            if constexpr (std::is_same_v<PlainType, bool>) {
                store_bool_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, int8_t>) {
                store_int8_t_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, uint8_t>) {
                store_uint8_t_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, int16_t>) {
                store_int16_t_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, uint16_t>) {
                store_uint16_t_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, int32_t>) {
                store_int32_t_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, uint32_t>) {
                store_uint32_t_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, int64_t>) {
                store_int64_t_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, uint64_t>) {
                store_uint64_t_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, float>) {
                store_float_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, double>) {
                store_double_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, long double>) {
                store_long_double_argument(index, value);
            } else if constexpr (std::is_same_v<PlainType, std::string>) {
                store_string_argument(index, value);
            } else {
                auto t = type_data::get(
                    mge::script::make_type_identifier<PlainType>());
                if (!t) {
                    MGE_THROW(illegal_argument)
                        << "Type " << type_name<PlainType>()
                        << " is not registered";
                }
                store_object_argument(index, &value, t);
            }
        }

        template <typename R>
            requires(!std::is_same_v<R, void>)
        R invoke_method(const char* method)
        {
            call_method(method);
            return get_result<R>();
        }

        template <typename R>
            requires(std::is_same_v<R, void>)
        R invoke_method(const char* method)
        {
            call_method(method);
        }

        template <typename R> R get_result()
        {
            if constexpr (std::is_same_v<R, bool>) {
                return get_bool_result();
            } else if constexpr (std::is_same_v<R, int8_t>) {
                return get_int8_t_result();
            } else if constexpr (std::is_same_v<R, uint8_t>) {
                return get_uint8_t_result();
            } else if constexpr (std::is_same_v<R, int16_t>) {
                return get_int16_t_result();
            } else if constexpr (std::is_same_v<R, uint16_t>) {
                return get_uint16_t_result();
            } else if constexpr (std::is_same_v<R, int32_t>) {
                return get_int32_t_result();
            } else if constexpr (std::is_same_v<R, uint32_t>) {
                return get_uint32_t_result();
            } else if constexpr (std::is_same_v<R, int64_t>) {
                return get_int64_t_result();
            } else if constexpr (std::is_same_v<R, uint64_t>) {
                return get_uint64_t_result();
            } else if constexpr (std::is_same_v<R, float>) {
                return get_float_result();
            } else if constexpr (std::is_same_v<R, double>) {
                return get_double_result();
            } else if constexpr (std::is_same_v<R, long double>) {
                return get_long_double_result();
            } else if constexpr (std::is_same_v<R, std::string>) {
                return get_string_result();
            } else {
                MGE_THROW_NOT_IMPLEMENTED << "Return type " << type_name<R>()
                                          << " not supported";
            }
        }

    protected:
        /**
         * @brief Check if a method is implemented.
         *
         * Check if a method is implemented in the scripting language. If a
         * method is not implemented, the base class will be called.
         *
         * @param method method name
         */
        virtual bool call_implemented(const char* method)
        {
            return true;
        }

        virtual void store_bool_argument(size_t index, bool value) = 0;
        virtual void store_int8_t_argument(size_t index, int8_t value) = 0;
        virtual void store_uint8_t_argument(size_t index, uint8_t value) = 0;
        virtual void store_int16_t_argument(size_t index, int16_t value) = 0;
        virtual void store_uint16_t_argument(size_t index, uint16_t value) = 0;
        virtual void store_int32_t_argument(size_t index, int32_t value) = 0;
        virtual void store_uint32_t_argument(size_t index, uint32_t value) = 0;
        virtual void store_int64_t_argument(size_t index, int64_t value) = 0;
        virtual void store_uint64_t_argument(size_t index, uint64_t value) = 0;
        virtual void store_float_argument(size_t index, float value) = 0;
        virtual void store_double_argument(size_t index, double value) = 0;
        virtual void store_long_double_argument(size_t      index,
                                                long double value) = 0;
        virtual void store_string_argument(size_t             index,
                                           const std::string& value) = 0;
        virtual void store_object_argument(size_t               index,
                                           void*                value,
                                           const type_data_ref& t) = 0;

        virtual call_result_type call_method(const char* method) = 0;

        virtual bool        get_bool_result() = 0;
        virtual int8_t      get_int8_t_result() = 0;
        virtual uint8_t     get_uint8_t_result() = 0;
        virtual int16_t     get_int16_t_result() = 0;
        virtual uint16_t    get_uint16_t_result() = 0;
        virtual int32_t     get_int32_t_result() = 0;
        virtual uint32_t    get_uint32_t_result() = 0;
        virtual int64_t     get_int64_t_result() = 0;
        virtual uint64_t    get_uint64_t_result() = 0;
        virtual float       get_float_result() = 0;
        virtual double      get_double_result() = 0;
        virtual long double get_long_double_result() = 0;
        virtual std::string get_string_result() = 0;
    };

} // namespace mge::script