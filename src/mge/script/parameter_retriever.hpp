#pragma once
#include "mge/script/call_context.hpp"

#include <cstdint>
#include <string_view>
#include <type_traits>

namespace mge::script {

    template <typename T, typename = void> struct parameter_retriever
    {};

#define MGE_DEFINE_RETRIEVER(RETRIEVED_TYPE)                                   \
    template <> struct parameter_retriever<RETRIEVED_TYPE, void>               \
    {                                                                          \
        static RETRIEVED_TYPE get(call_context& context, size_t position)      \
        {                                                                      \
            return context.RETRIEVED_TYPE##_parameter(position);               \
        }                                                                      \
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
        static std::string get(call_context& context, size_t position)
        {
            return context.string_parameter(position);
        }
    };

    template <typename T>
    struct parameter_retriever<
        T,
        typename std::enable_if<
            std::is_same_v<typename std::remove_cv<T>::type, std::string_view>,
            void>::type>
    {
        static std::string_view get(call_context& context, size_t position)
        {
            return context.string_view_parameter(position);
        }
    };

    template <typename T>
    struct parameter_retriever<
        T,
        typename std::enable_if<
            std::is_enum<typename std::remove_cv<T>::type>::value,
            void>::type>
    {
        static T get(call_context& context, size_t position)
        {
            return static_cast<T>(
                parameter_retriever<std::underlying_type<T>::type>::get(
                    context,
                    position));
        }
    };

    template <typename T> struct parameter_retriever<const T&, void>
    {
        static const T& get(call_context& context, size_t position)
        {
            MGE_THROW_NOT_IMPLEMENTED;
        }
    };

    template <typename T> struct parameter_retriever<T&&, void>
    {
        static const T& get(call_context& context, size_t position)
        {
            MGE_THROW_NOT_IMPLEMENTED;
        }
    };

} // namespace mge::script