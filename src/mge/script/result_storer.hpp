// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/is_shared_ptr.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/script/call_context.hpp"
#include <memory>
#include <type_traits>

namespace mge::script {

    template <typename T, typename = void> struct result_storer
    {};

#define MGE_DEFINE_STORER(STORED_TYPE)                                         \
    template <> struct result_storer<STORED_TYPE, void>                        \
    {                                                                          \
        static void store(call_context& context, STORED_TYPE value)            \
        {                                                                      \
            context.store_##STORED_TYPE##_result(value);                       \
        }                                                                      \
    }

    MGE_DEFINE_STORER(bool);
    MGE_DEFINE_STORER(int8_t);
    MGE_DEFINE_STORER(uint8_t);
    MGE_DEFINE_STORER(int16_t);
    MGE_DEFINE_STORER(uint16_t);
    MGE_DEFINE_STORER(int32_t);
    MGE_DEFINE_STORER(uint32_t);
    MGE_DEFINE_STORER(int64_t);
    MGE_DEFINE_STORER(uint64_t);
    MGE_DEFINE_STORER(float);
    MGE_DEFINE_STORER(double);

    template <typename T> struct result_storer<std::shared_ptr<T>, void>
    {
        static void store(call_context&             context,
                          const std::shared_ptr<T>& value)
        {
            MGE_THROW_NOT_IMPLEMENTED;
        }
    };

    template <typename T>
    struct result_storer<
        T,
        typename std::enable_if<
            std::is_enum<typename std::remove_cv<T>::type>::value,
            void>::type>
    {
        static void store(call_context& context, T value)
        {
            result_storer<std::underlying_type<T>::type>::store(
                context,
                static_cast<std::underlying_type<T>::type>(value));
        }
    };

    template <typename T> struct result_storer<T&, void>
    {
        static void store(call_context& context, T& value)
        {
            MGE_THROW_NOT_IMPLEMENTED;
        }
    };

    // sink implementation for generic objects
    template <typename T>
    struct result_storer<
        T,
        typename std::enable_if<
            !is_shared_ptr_v<typename std::remove_cv<T>::type> &&
                std::is_class<typename std::remove_cv<T>::type>::value,
            void>::type>
    {
        static void store(call_context& context, const T& value)
        {
            MGE_THROW_NOT_IMPLEMENTED;
        }
    };

} // namespace mge::script