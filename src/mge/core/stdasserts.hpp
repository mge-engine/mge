#pragma once
#include "mge/core/stdexceptions.hpp"

#define MGE_ASSERT_ARG_NOT_NULL(arg)                                           \
    do {                                                                       \
        if (arg == nullptr) {                                                  \
            MGE_THROW(::mge::illegal_argument)                                 \
                << "Argument '" << #arg << "' must not be null";               \
        }                                                                      \
    } while (false)