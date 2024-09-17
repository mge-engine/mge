// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_data.hpp"
#include "mge/script/type_identifier.hpp"
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

        template <typename T>
            requires std::is_enum_v<T>
        void result(T value)
        {
            type_data_ref t = type_data::get(make_type_identifier<T>());
            object_result(&value, t);
        }

        template <typename T> void result(T value)
        {
            using PlainType = std::remove_cv_t<T>;
            if constexpr (std::is_same_v<PlainType, bool>) {
                bool_result(value);
            } else if constexpr (std::is_same_v<PlainType, int8_t> ||
                                 std::is_same_v<PlainType, signed char> ||
                                 std::is_same_v<PlainType, char>) {
                int8_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, uint8_t>) {
                uint8_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, int16_t>) {
                int16_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, uint16_t>) {
                uint16_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, int32_t>) {
                int32_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, uint32_t>) {
                uint32_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, int64_t>) {
                int64_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, uint64_t>) {
                uint64_t_result(value);
            } else if constexpr (std::is_same_v<PlainType, float>) {
                float_result(value);
            } else if constexpr (std::is_same_v<PlainType, double>) {
                double_result(value);
            } else if constexpr (std::is_same_v<PlainType, long double>) {
                long_double_result(value);
            } else if constexpr (std::is_same_v<PlainType, std::string> ||
                                 std::is_same_v<T, const std::string&> ||
                                 std::is_same_v<T, const char*>) {
                string_result(value);
            } else if constexpr (std::is_same_v<T, std::string_view>) {
                string_result(std::string(value.begin(), value.end()));
            } else if constexpr (std::is_same_v<PlainType, std::wstring> ||
                                 std::is_same_v<T, const std::wstring&>) {
                wstring_result(value);
            } else {
                auto t = type_data::get(make_type_identifier<PlainType>());
                if (!t) {
                    MGE_THROW(illegal_argument)
                        << "Type " << type_name<PlainType>()
                        << " is not registered";
                }
                object_result(&value, t);
            }
        }

        virtual void* this_ptr() = 0;
        virtual void* shared_ptr_address() = 0;

        virtual void bool_result(bool result) = 0;
        virtual void int8_t_result(int8_t result) = 0;
        virtual void uint8_t_result(uint8_t result) = 0;
        virtual void int16_t_result(int16_t result) = 0;
        virtual void uint16_t_result(uint16_t result) = 0;
        virtual void int32_t_result(int32_t result) = 0;
        virtual void uint32_t_result(uint32_t result) = 0;
        virtual void int64_t_result(int64_t result) = 0;
        virtual void uint64_t_result(uint64_t result) = 0;
        virtual void float_result(float result) = 0;
        virtual void double_result(double result) = 0;
        virtual void long_double_result(long double result) = 0;
        virtual void string_result(const std::string& result) = 0;
        virtual void wstring_result(const std::wstring& result) = 0;
        virtual void object_result(void* result, const type_data_ref& t) = 0;

        virtual void exception_thrown(const std::exception& e) = 0;
        virtual void exception_thrown(const mge::exception& e) = 0;
        virtual void exception_thrown() = 0;

        template <typename T> T parameter(size_t index)
        {
            if constexpr (std::is_same_v<T, bool>) {
                return bool_parameter(index);
            } else if constexpr (std::is_same_v<T, int8_t>) {
                return static_cast<T>(int8_t_parameter(index));
            } else if constexpr (std::is_same_v<T, uint8_t>) {
                return static_cast<T>(uint8_t_parameter(index));
            } else if constexpr (std::is_same_v<T, int16_t>) {
                return static_cast<T>(int16_t_parameter(index));
            } else if constexpr (std::is_same_v<T, uint16_t>) {
                return static_cast<T>(uint16_t_parameter(index));
            } else if constexpr (std::is_same_v<T, int32_t>) {
                return static_cast<T>(int32_t_parameter(index));
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                return static_cast<T>(uint32_t_parameter(index));
            } else if constexpr (std::is_same_v<T, int64_t>) {
                return static_cast<T>(int64_t_parameter(index));
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                return static_cast<T>(uint64_t_parameter(index));
            } else if constexpr (std::is_same_v<T, float>) {
                return static_cast<T>(float_parameter(index));
            } else if constexpr (std::is_same_v<T, double>) {
                return static_cast<T>(double_parameter(index));
            } else if constexpr (std::is_same_v<T, long double>) {
                return static_cast<T>(long_double_parameter(index));
            } else if constexpr (std::is_same_v<T, std::string>) {
                return string_parameter(index);
            } else {
                MGE_THROW_NOT_IMPLEMENTED << "Parameter type " << type_name<T>()
                                          << " not supported";
            }
        }

        virtual bool        bool_parameter(size_t index) = 0;
        virtual int8_t      int8_t_parameter(size_t index) = 0;
        virtual uint8_t     uint8_t_parameter(size_t index) = 0;
        virtual int16_t     int16_t_parameter(size_t index) = 0;
        virtual uint16_t    uint16_t_parameter(size_t index) = 0;
        virtual int32_t     int32_t_parameter(size_t index) = 0;
        virtual uint32_t    uint32_t_parameter(size_t index) = 0;
        virtual int64_t     int64_t_parameter(size_t index) = 0;
        virtual uint64_t    uint64_t_parameter(size_t index) = 0;
        virtual float       float_parameter(size_t index) = 0;
        virtual double      double_parameter(size_t index) = 0;
        virtual long double long_double_parameter(size_t index) = 0;
        virtual std::string string_parameter(size_t index) = 0;
    };

} // namespace mge::script