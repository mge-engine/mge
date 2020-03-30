// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {
    enum class filter_function : uint8_t
    {
        NEAREST,
        LINEAR
    };

    MGE_GRAPHICS_EXPORT std::ostream &operator<<(std::ostream &         os,
                                                 const filter_function &f);
} // namespace mge
