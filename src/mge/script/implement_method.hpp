// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/identity_type.hpp"
#include <cstddef>
/*
namespace mge {
    constexpr std::size_t count_spaces(const char* str, std::size_t count = 0)
    {
        return *str == '\0'
                   ? count
                   : count_spaces(str + 1, count + (*str == ' ' ? 1 : 0));
    }
} // namespace mge
*/

#define MGE_INTERNAL_20TH_ARG(_1,                                              \
                              _2,                                              \
                              _3,                                              \
                              _4,                                              \
                              _5,                                              \
                              _6,                                              \
                              _7,                                              \
                              _8,                                              \
                              _9,                                              \
                              _10,                                             \
                              _11,                                             \
                              _12,                                             \
                              _13,                                             \
                              _14,                                             \
                              _15,                                             \
                              _16,                                             \
                              _17,                                             \
                              _18,                                             \
                              _19,                                             \
                              _20,                                             \
                              ...)                                             \
    _20

#define MGE_INTERNAL_COUNT_ARGS(...)                                           \
    MGE_INTERNAL_20TH_ARG(__VA_ARGS__,                                         \
                          19,                                                  \
                          18,                                                  \
                          17,                                                  \
                          16,                                                  \
                          15,                                                  \
                          14,                                                  \
                          13,                                                  \
                          12,                                                  \
                          11,                                                  \
                          10,                                                  \
                          9,                                                   \
                          8,                                                   \
                          7,                                                   \
                          6,                                                   \
                          5,                                                   \
                          4,                                                   \
                          3,                                                   \
                          2,                                                   \
                          1,                                                   \
                          0)

#define MGE_INTERNAL_ASSERT_PARENTHESIS(...)                                   \
    static_assert(MGE_INTERNAL_COUNT_ARGS(__VA_ARGS__) == 1,                   \
                  "Arguments must be enclosed in parenthesis")

#define MGE_INTERNAL_EXTRACT_VARNAME(type_and_name)

#define MGE_IMPLEMENT_METHOD(RETURN_TYPE, METHOD_NAME, ARGS, QUALIFIERS)       \
    mge::identity_type<RETURN_TYPE> METHOD_NAME ARGS QUALIFIERS                \
    {                                                                          \
        MGE_INTERNAL_ASSERT_PARENTHESIS(ARGS);                                 \
                                                                               \
        if (!m_context) {                                                      \
            MGE_THROW(mge::illegal_state)                                      \
                << "No context set for call to '" << #METHOD_NAME << "'";      \
        }                                                                      \
                                                                               \
        if constexpr (std::is_same_v<RETURN_TYPE, void>) {                     \
            /* void type */                                                    \
            m_context->invoke_method<void>(#METHOD_NAME);                      \
            return;                                                            \
        } else {                                                               \
            /* non void */                                                     \
            return m_context->invoke_method<RETURN_TYPE>(#METHOD_NAME);        \
        }                                                                      \
    }
