// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"

#include <memory>
#include <map>
#include <type_traits>

namespace mge {

    /**
     * A script context provides the frame around the execution of once
     * or more scripts.
     *
     * It provides access to objects defined in the script (or predefined
     * in the interpreter) (globally defined synbols, and means to execute
     * code.
     */
    class MGE_SCRIPT_EXPORT script_context
            : public noncopyable,
              public std::enable_shared_from_this<script_context>
    {
    protected:
        /**
         * Constructor.
         */
        script_context() = default;
    public:
        /**
         * Destructor.
         */
        virtual ~script_context() = default;

        /**
         * Evaluates a script.
         * @param script script code
         */
        virtual void eval(const char *script) = 0;

        template <typename T>
        typename std::enable_if<std::is_same<T, signed char>::value, T>::type
        value(const char *name)
        {
            return signed_char_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, unsigned char>::value, T>::type
        value(const char *name)
        {
            return unsigned_char_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, char>::value, T>::type
        value(const char *name)
        {
            return char_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, short>::value, T>::type
        value(const char *name)
        {
            return short_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, unsigned short>::value, T>::type
        value(const char *name)
        {
            return unsigned_short_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, int>::value, T>::type
        value(const char *name)
        {
            return int_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, unsigned int>::value, T>::type
        value(const char *name)
        {
            return unsigned_int_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, long>::value, T>::type
        value(const char *name)
        {
            return long_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, unsigned long>::value, T>::type
        value(const char *name)
        {
            return unsigned_long_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, long long>::value, T>::type
        value(const char *name)
        {
            return long_long_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, unsigned long long>::value, T>::type
        value(const char *name)
        {
            return unsigned_long_long_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<
            typename std::remove_const<typename std::remove_reference<T>::type>::type,
            std::string>::value, T>::type
        value(const char *name)
        {
            return string_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, bool>::value, T>::type
        value(const char *name)
        {
            return bool_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, float>::value, T>::type
        value(const char *name)
        {
            return float_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, double>::value, T>::type
        value(const char *name)
        {
            return double_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, long double>::value, T>::type
        value(const char *name)
        {
            return long_double_value(name);
        }

        template <typename T>
        typename std::enable_if<std::is_enum<T>::value, T>::type
        value(const char *name)
        {
            typename std::underlying_type<T>::type r;
            r = value<decltype(r)>(name);
            return (T)r;
        }
    protected:
        virtual signed char signed_char_value(const char *name) = 0;
        virtual char char_value(const char *name) = 0;
        virtual unsigned char unsigned_char_value(const char *name) = 0;
        virtual short short_value(const char *name) = 0;
        virtual unsigned short unsigned_short_value(const char *name) = 0;
        virtual int int_value(const char *name) = 0;
        virtual unsigned int unsigned_int_value(const char *name) = 0;
        virtual long long_value(const char *name) = 0;
        virtual unsigned long unsigned_long_value(const char *name) = 0;
        virtual long long long_long_value(const char *name) = 0;
        virtual unsigned long long unsigned_long_long_value(const char *name) = 0;
        virtual std::string string_value(const char *name) = 0;
        virtual bool bool_value(const char *name) = 0;
        virtual float float_value(const char *name) = 0;
        virtual double double_value(const char *name) = 0;
        virtual long double long_double_value(const char *name) = 0;
    };


}
