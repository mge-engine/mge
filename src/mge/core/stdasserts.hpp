// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"

#define MGE_ASSERT_ARGUMENT_NOT_NULL(arg)                                      \
    do {                                                                       \
        if (arg == nullptr) {                                                  \
            MGE_THROW(mge::illegal_argument)                                   \
                << "Argument '" << #arg << "' must not be null";               \
        }                                                                      \
    } while (false)
