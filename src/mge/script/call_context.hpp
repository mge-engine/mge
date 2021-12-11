// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include <string>
#include <type_traits>

namespace mge::script {
    /**
     * @brief A call context is used when invoking functions or methods.
     *
     */
    class MGESCRIPT_EXPORT call_context
    {
    public:
        call_context() = default;
        virtual ~call_context() = default;

        template <typename T>
        typename std::enable_if<std::is_same<T, bool>::value, T>::type
        parameter(size_t position)
        {
            return bool_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, char>::value, T>::type
        parameter(size_t position)
        {
            return char_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, signed char>::value, T>::type
        parameter(size_t position)
        {
            return char_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, unsigned char>::value, T>::type
        parameter(size_t position)
        {
            return unsigned_char_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, short>::value, T>::type
        parameter(size_t position)
        {
            return short_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, unsigned short>::value, T>::type
        parameter(size_t position)
        {
            return unsigned_short_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, int>::value, T>::type
        parameter(size_t position)
        {
            return int_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, unsigned int>::value, T>::type
        parameter(size_t position)
        {
            return unsigned_int_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, int64_t>::value, T>::type
        parameter(size_t position)
        {
            return long_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, uint64_t>::value, T>::type
        parameter(size_t position)
        {
            return unsigned_long_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, float>::value, T>::type
        parameter(size_t position)
        {
            return float_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, double>::value, T>::type
        parameter(size_t position)
        {
            return double_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, std::string>::value, T>::type
        parameter(size_t position)
        {
            return string_parameter(position);
        }

        template <typename T> void store_result(T result)
        {
            static_assert(false);
        }

        template <> void store_result<bool>(bool result)
        {
            store_bool_result(result);
        }

        template <> void store_result<char>(char result)
        {
            store_char_result(result);
        }

        template <> void store_result<signed char>(signed char result)
        {
            store_signed_char_result(result);
        }

        template <> void store_result<unsigned char>(unsigned char result)
        {
            store_unsigned_char_result(result);
        }

        template <> void store_result<short>(short result)
        {
            store_short_result(result);
        }

        template <> void store_result<unsigned short>(unsigned short result)
        {
            store_unsigned_short_result(result);
        }

        template <> void store_result<int>(int result)
        {
            store_int_result(result);
        }

        template <> void store_result<unsigned int>(unsigned int result)
        {
            store_unsigned_int_result(result);
        }

        template <> void store_result<int64_t>(int64_t result)
        {
            store_long_result(result);
        }

        template <> void store_result<uint64_t>(uint64_t result)
        {
            store_unsigned_long_result(result);
        }

        template <> void store_result<float>(float result)
        {
            store_float_result(result);
        }

        template <> void store_result<double>(double result)
        {
            store_double_result(result);
        }

        template <>
        void store_result<const std::string&>(const std::string& result)
        {
            store_string_result(result);
        }

    protected:
        virtual bool           bool_parameter(size_t position) = 0;
        virtual char           char_parameter(size_t position) = 0;
        virtual unsigned char  unsigned_char_parameter(size_t position) = 0;
        virtual short          short_parameter(size_t position) = 0;
        virtual unsigned short unsigned_short_parameter(size_t position) = 0;
        virtual int            int_parameter(size_t position) = 0;
        virtual unsigned int   unsigned_int_parameter(size_t position) = 0;
        virtual int64_t        long_parameter(size_t position) = 0;
        virtual uint64_t       unsigned_long_parameter(size_t position) = 0;
        virtual float          float_parameter(size_t position) = 0;
        virtual double         double_parameter(size_t position) = 0;
        virtual std::string    string_parameter(size_t position) = 0;

        virtual void store_bool_result(bool result) = 0;
        virtual void store_char_result(char result) = 0;
        virtual void store_signed_char_result(signed char result) = 0;
        virtual void store_unsigned_char_result(unsigned char result) = 0;
        virtual void store_short_result(short result) = 0;
        virtual void store_unsigned_short_result(unsigned short result) = 0;
        virtual void store_int_result(int result) = 0;
        virtual void store_unsigned_int_result(unsigned int result) = 0;
        virtual void store_long_result(int64_t result) = 0;
        virtual void store_unsigned_long_result(uint64_t result) = 0;
        virtual void store_float_result(float result) = 0;
        virtual void store_double_result(double result) = 0;
        virtual void store_string_result(const std::string& result) = 0;
    };
} // namespace mge::script