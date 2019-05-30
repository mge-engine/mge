// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <iosfwd>
#include <cstdint>

namespace mge {
    enum class MGE_GRAPHICS_EXPORT buffer_access : uint8_t
    {
        NONE        = 0,
        READ        = 1,
        WRITE       = 2,
        READ_WRITE  = 3
    };

    MGE_GRAPHICS_EXPORT std::ostream& operator<<(std::ostream& os, const buffer_access a);
}
