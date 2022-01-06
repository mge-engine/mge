// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"

#include <functional>
#include <string>
#include <type_traits>
#include <typeindex>

namespace mge::script {

    /**
     * @brief Call context.
     *
     * Used to interface with parameter retrieval by a function invocation.
     *
     */
    class MGESCRIPT_EXPORT call_context
    {
    public:
        virtual ~call_context();

        /**
         * @brief Retrieve this pointer for current context.
         *
         * @return this pointer, nullpointer if not in a method context
         */
        virtual void* this_ptr() = 0;

        /**
         * @brief Retrieve bool parameter.
         * @param position parameter position (0-based)
         */
        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, bool>::value,
            T>::type
        parameter(size_t position)
        {
            return bool_parameter(position);
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, char>::value,
            T>::type
        parameter(size_t position)
        {
            return int8_parameter(position);
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, signed char>::value,
            T>::type
        parameter(size_t position)
        {
            return int8_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<typename std::remove_cv<T>::type,
                                             unsigned char>::value,
                                T>::type
        parameter(size_t position)
        {
            return uint8_parameter(position);
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, short>::value,
            T>::type
        parameter(size_t position)
        {
            return int16_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<typename std::remove_cv<T>::type,
                                             unsigned short>::value,
                                T>::type
        parameter(size_t position)
        {
            return uint16_parameter(position);
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, int>::value,
            T>::type
        parameter(size_t position)
        {
            return int32_parameter(position);
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, unsigned int>::value,
            T>::type
        parameter(size_t position)
        {
            return uint32_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<typename std::remove_cv<T>::type,
                                             unsigned long>::value,
                                T>::type
        parameter(size_t position)
        {
            if constexpr (sizeof(T) == 4) {
                return uint32_parameter(position);
            } else {
                return uint64_parameter(position);
            }
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, long>::value,
            T>::type
        parameter(size_t position)
        {
            if constexpr (sizeof(T) == 4) {
                return int32_parameter(position);
            } else {
                return int64_parameter(position);
            }
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, int64_t>::value,
            T>::type
        parameter(size_t position)
        {
            return int64_parameter(position);
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, uint64_t>::value,
            T>::type
        parameter(size_t position)
        {
            return uint64_parameter(position);
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, float>::value,
            T>::type
        parameter(size_t position)
        {
            return float_parameter(position);
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, double>::value,
            T>::type
        parameter(size_t position)
        {
            return double_parameter(position);
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<typename std::remove_cv<T>::type, std::string>::value,
            T>::type
        parameter(size_t position)
        {
            return string_parameter(position);
        }

        template <typename T> void store_result(T result)
        {
            static_assert(false, "Default result store method called");
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

        template <> void store_result<long>(long result)
        {
            store_long_result(result);
        }

        template <> void store_result<unsigned long>(unsigned long result)
        {
            store_unsigned_long_result(result);
        }

        template <> void store_result<int64_t>(int64_t result)
        {
            store_int64_result(result);
        }

        template <> void store_result<uint64_t>(uint64_t result)
        {
            store_uint64_result(result);
        }

        template <> void store_result<float>(float result)
        {
            store_float_result(result);
        }

        template <> void store_result<double>(double result)
        {
            store_double_result(result);
        }

        void store_result(const std::string& result)
        {
            store_string_result(result);
        }

    protected:
        virtual bool           bool_parameter(size_t position) = 0;
        virtual char           int8_parameter(size_t position) = 0;
        virtual unsigned char  uint8_parameter(size_t position) = 0;
        virtual short          int16_parameter(size_t position) = 0;
        virtual unsigned short uint16_parameter(size_t position) = 0;
        virtual int            int32_parameter(size_t position) = 0;
        virtual unsigned int   uint32_parameter(size_t position) = 0;
        virtual int64_t        int64_parameter(size_t position) = 0;
        virtual uint64_t       uint64_parameter(size_t position) = 0;
        virtual float          float_parameter(size_t position) = 0;
        virtual double         double_parameter(size_t position) = 0;
        virtual std::string    string_parameter(size_t position) = 0;
        virtual void* object_parameter(size_t position, std::type_index ti) = 0;

        virtual void store_bool_result(bool result) = 0;
        virtual void store_char_result(char result) = 0;
        virtual void store_signed_char_result(signed char result) = 0;
        virtual void store_unsigned_char_result(unsigned char result) = 0;
        virtual void store_short_result(short result) = 0;
        virtual void store_unsigned_short_result(unsigned short result) = 0;
        virtual void store_int_result(int result) = 0;
        virtual void store_unsigned_int_result(unsigned int result) = 0;
        virtual void store_long_result(long result) = 0;
        virtual void store_unsigned_long_result(unsigned long result) = 0;
        virtual void store_int64_result(int64_t result) = 0;
        virtual void store_uint64_result(uint64_t result) = 0;
        virtual void store_float_result(float result) = 0;
        virtual void store_double_result(double result) = 0;
        virtual void store_string_result(const std::string& result) = 0;
    };

    /**
     * Invocation function that uses a call context for
     * parameter retrieval and result storage.
     */
    using invoke_function = std::function<void(call_context&)>;
} // namespace mge::script