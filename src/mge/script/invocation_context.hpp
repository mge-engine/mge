// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    class MGESCRIPT_EXPORT invocation_context
    {
    public:
        invocation_context() = default;
        virtual ~invocation_context() = default;

        template <typenameT> void store_argument(size_t index, T value)
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
                auto t = type_data::get(typeid(PlainType));
                if (!t) {
                    MGE_THROW(illegal_argument)
                        << "Type " << type_name<PlainType>()
                        << " is not registered";
                }
                store_object_argument(index, &value, t);
            }
        }

        template <typename R> R invoke_method(const char* method)
        {
            call(method);
            return get_result<R>();
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

        template <typename R, typename... Args>
        R call(const char* method, Args... args)
        {
            size_t index = 0;
            (store_argument<Args>(index++, args), ...);
            return invoke_method<R>(method);
        }

        virtual void call(const char* method) = 0;

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