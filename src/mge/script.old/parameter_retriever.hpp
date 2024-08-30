// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/trace.hpp"
#include "mge/script/call_context.hpp"

#include <cstdint>
#include <string_view>
#include <type_traits>

namespace mge {
#ifdef BUILD_SCRIPT
    MGE_USE_TRACE(SCRIPT);
#else
    MGE_USE_IMPORTED_TRACE(SCRIPT);
#endif
} // namespace mge

namespace mge::script {

    template <typename T, typename = void> struct parameter_retriever
    {};

#define MGE_DEFINE_RETRIEVER(RETRIEVED_TYPE)                                   \
    template <> struct parameter_retriever<RETRIEVED_TYPE, void>               \
    {                                                                          \
        parameter_retriever(call_context& context_, size_t position_)          \
            : context(context_)                                                \
            , position(position_)                                              \
        {}                                                                     \
                                                                               \
        RETRIEVED_TYPE get()                                                   \
        {                                                                      \
            return context.RETRIEVED_TYPE##_parameter(position);               \
        }                                                                      \
                                                                               \
        call_context& context;                                                 \
        size_t        position;                                                \
    }

    MGE_DEFINE_RETRIEVER(bool);
    MGE_DEFINE_RETRIEVER(int8_t);
    MGE_DEFINE_RETRIEVER(uint8_t);
    MGE_DEFINE_RETRIEVER(int16_t);
    MGE_DEFINE_RETRIEVER(uint16_t);
    MGE_DEFINE_RETRIEVER(int32_t);
    MGE_DEFINE_RETRIEVER(uint32_t);
    MGE_DEFINE_RETRIEVER(int64_t);
    MGE_DEFINE_RETRIEVER(uint64_t);
    MGE_DEFINE_RETRIEVER(float);
    MGE_DEFINE_RETRIEVER(double);

    template <typename T>
    struct parameter_retriever<
        T,
        typename std::enable_if<
            std::is_same_v<typename std::remove_cv<T>::type, std::string>,
            void>::type>
    {
        parameter_retriever(call_context& context_, size_t position_)
            : context(context_)
            , position(position)
        {}

        std::string get() { return context.string_parameter(position); }

        call_context& context;
        size_t        position;
    };

    template <typename T>
    struct parameter_retriever<
        T,
        typename std::enable_if<
            std::is_same_v<typename std::remove_cv<T>::type, std::string_view>,
            void>::type>
    {
        parameter_retriever(call_context& context_, size_t position_)
            : context(context_)
            , position(position)
        {}

        std::string_view get()
        {
            value = context.string_parameter(position);
            return std::string_view(value.begin(), value.end());
        }

        call_context& context;
        size_t        position;
        std::string   value;
    };

    template <> struct parameter_retriever<const char*, void>
    {
        parameter_retriever(call_context& context_, size_t position_)
            : context(context_)
            , position(position_)
        {}

        const char* get()
        {
            value = context.string_parameter(position);
            return value.c_str();
        }

        call_context& context;
        size_t        position;
        std::string   value;
    };

    template <typename T>
    struct parameter_retriever<
        T,
        typename std::enable_if<
            std::is_enum<typename std::remove_cv<T>::type>::value,
            void>::type>
        : parameter_retriever<typename std::underlying_type<T>::type, void>
    {
        parameter_retriever(call_context& context_, size_t position_)
            : parameter_retriever<std::underlying_type<T>::type, void>(
                  context_, position_)
        {}

        T get()
        {
            return static_cast<T>(
                parameter_retriever<std::underlying_type<T>::type>::get());
        }
    };

    template <typename T> struct parameter_retriever<const T&, void>
    {
        parameter_retriever(call_context& context_, size_t position_)
            : context(context_)
            , position(position)
        {}

        const T& get() { MGE_THROW_NOT_IMPLEMENTED; }

        call_context& context;
        size_t        position;
    };

    template <typename T> struct parameter_retriever<T&&, void>
    {
        parameter_retriever(call_context& context_, size_t position_)
            : context(context_)
            , position(position)
        {}

        const T& get() { MGE_THROW_NOT_IMPLEMENTED; }

        call_context& context;
        size_t        position;
    };

    template <> struct parameter_retriever<void*, void>
    {
        parameter_retriever(call_context& context_, size_t position_)
            : context(context_)
            , position(position_)
        {}

        void* get() { MGE_THROW_NOT_IMPLEMENTED; }

        call_context& context;
        size_t        position;
    };

} // namespace mge::script