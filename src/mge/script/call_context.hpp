// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_data.hpp"
#include <functional>
#include <string>
#include <type_traits>

namespace mge::script {

    /**
     * Context of a call.
     *
     * When a script calls a function or method, is used to request the
     * parameter values.
     */
    class MGESCRIPT_EXPORT call_context
    {
    protected:
        call_context() = default;

    public:
        virtual ~call_context() = default;

        template <typename T> static constexpr bool is_simple_result()
        {
            using PlainType = std::remove_cv_t<T>;
            return std::is_same_v<PlainType, bool> ||
                   std::is_same_v<PlainType, signed char> ||
                   std::is_same_v<PlainType, char> ||
                   std::is_same_v<PlainType, int8_t> ||
                   std::is_same_v<PlainType, uint8_t> ||
                   std::is_same_v<PlainType, int16_t> ||
                   std::is_same_v<PlainType, uint16_t> ||
                   std::is_same_v<PlainType, int32_t> ||
                   std::is_same_v<PlainType, uint32_t> ||
                   std::is_same_v<PlainType, int64_t> ||
                   std::is_same_v<PlainType, uint64_t> ||
                   std::is_same_v<PlainType, float> ||
                   std::is_same_v<PlainType, double> ||
                   std::is_same_v<PlainType, long double> ||
                   std::is_same_v<PlainType, std::string>;
        }

        template <typename T> void store_result(T value)
        {
            using PlainType = std::remove_cv_t<T>;
            if constexpr (std::is_same_v<PlainType, bool>) {
                store_bool_result(value);
            } else if constexpr (std::is_same_v<PlainType, int8_t> ||
                                 std::is_same_v<PlainType, signed char> ||
                                 std::is_same_v<PlainType, char>) {
                store_int8_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, uint8_t>) {
                store_uint8_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, int16_t>) {
                store_int16_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, uint16_t>) {
                store_uint16_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, int32_t>) {
                store_int32_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, uint32_t>) {
                store_uint32_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, int64_t>) {
                store_int64_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, uint64_t>) {
                store_uint64_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, float>) {
                store_float_result(value);
            } else if constexpr (std::is_same_v<PlainType, double>) {
                store_double_result(value);
            } else if constexpr (std::is_same_v<PlainType, long double>) {
                store_long_double_result(value);
            } else if constexpr (std::is_same_v<PlainType, std::string> ||
                                 std::is_same_v<T, const std::string&> ||
                                 std::is_same_v<T, std::string_view> ||
                                 std::is_same_v<T, const char*>) {
                store_string_result(value);
            } else if constexpr (std::is_same_v<PlainType, std::wstring> ||
                                 std::is_same_v<T, const std::wstring&>) {
                store_wstring_result(value);
            } else {
                auto t = type_data::get(make_type_identifier<PlainType>());
                if (!t) {
                    MGE_THROW(illegal_argument)
                        << "Type " << type_name<PlainType>()
                        << " is not registered";
                }
                store_object_result(&value, t);
            }
        }

        virtual void* get_this() = 0;

        virtual void store_bool_result(bool result) = 0;
        virtual void store_int8_t_result(int8_t result) = 0;
        virtual void store_uint8_t_result(uint8_t result) = 0;
        virtual void store_int16_t_result(int16_t result) = 0;
        virtual void store_uint16_t_result(uint16_t result) = 0;
        virtual void store_int32_t_result(int32_t result) = 0;
        virtual void store_uint32_t_result(uint32_t result) = 0;
        virtual void store_int64_t_result(int64_t result) = 0;
        virtual void store_uint64_t_result(uint64_t result) = 0;
        virtual void store_float_result(float result) = 0;
        virtual void store_double_result(double result) = 0;
        virtual void store_long_double_result(long double result) = 0;
        virtual void store_string_result(const std::string& result) = 0;
        virtual void store_wstring_result(const std::wstring& result) = 0;

        virtual void store_object_result(void*                result,
                                         const type_data_ref& t) = 0;

        virtual void exception_thrown(const std::exception& e) = 0;
        virtual void exception_thrown(const mge::exception& e) = 0;
        virtual void exception_thrown() = 0;

        template <typename T> T get_parameter(size_t index)
        {
            if constexpr (std::is_same_v<T, bool>) {
                return get_bool_parameter(index);
            } else if constexpr (std::is_same_v<T, int8_t>) {
                return static_cast<T>(get_int8_t_parameter(index));
            } else if constexpr (std::is_same_v<T, uint8_t>) {
                return static_cast<T>(get_uint8_t_parameter(index));
            } else if constexpr (std::is_same_v<T, int16_t>) {
                return static_cast<T>(get_int16_t_parameter(index));
            } else if constexpr (std::is_same_v<T, uint16_t>) {
                return static_cast<T>(get_uint16_t_parameter(index));
            } else if constexpr (std::is_same_v<T, int32_t>) {
                return static_cast<T>(get_int32_t_parameter(index));
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                return static_cast<T>(get_uint32_t_parameter(index));
            } else if constexpr (std::is_same_v<T, int64_t>) {
                return static_cast<T>(get_int64_t_parameter(index));
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                return static_cast<T>(get_uint64_t_parameter(index));
            } else if constexpr (std::is_same_v<T, float>) {
                return static_cast<T>(get_float_parameter(index));
            } else if constexpr (std::is_same_v<T, double>) {
                return static_cast<T>(get_double_parameter(index));
            } else if constexpr (std::is_same_v<T, long double>) {
                return static_cast<T>(get_long_double_parameter(index));
            } else if constexpr (std::is_same_v<T, std::string>) {
                return get_string_parameter(index);
            } else {
                MGE_THROW_NOT_IMPLEMENTED << "Parameter type " << type_name<T>()
                                          << " not supported";
            }
        }

        virtual bool        get_bool_parameter(size_t index) = 0;
        virtual int8_t      get_int8_t_parameter(size_t index) = 0;
        virtual uint8_t     get_uint8_t_parameter(size_t index) = 0;
        virtual int16_t     get_int16_t_parameter(size_t index) = 0;
        virtual uint16_t    get_uint16_t_parameter(size_t index) = 0;
        virtual int32_t     get_int32_t_parameter(size_t index) = 0;
        virtual uint32_t    get_uint32_t_parameter(size_t index) = 0;
        virtual int64_t     get_int64_t_parameter(size_t index) = 0;
        virtual uint64_t    get_uint64_t_parameter(size_t index) = 0;
        virtual float       get_float_parameter(size_t index) = 0;
        virtual double      get_double_parameter(size_t index) = 0;
        virtual long double get_long_double_parameter(size_t index) = 0;
        virtual std::string get_string_parameter(size_t index) = 0;
    };

} // namespace mge::script