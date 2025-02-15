// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <cstddef>

namespace mge {
    constexpr std::size_t count_spaces(const char* str, std::size_t count = 0)
    {
        return *str == '\0'
                   ? count
                   : count_spaces(str + 1, count + (*str == ' ' ? 1 : 0));
    }
} // namespace mge

#define MGE_IMPLEMENT_METHOD(RETURN_TYPE, METHOD_NAME, ARGS, QUALIFIERS)       \
    RETURN_TYPE METHOD_NAME ARGS QUALIFIERS                                    \
    {                                                                          \
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
