// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/input/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {

    enum class MGE_INPUT_EXPORT key_action
    {
        PRESS   = 0,
        RELEASE = 1,
        REPEAT  = 2
    };

    MGE_INPUT_EXPORT std::ostream &operator<<(std::ostream &    os,
                                              const key_action &a);
} // namespace mge
