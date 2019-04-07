// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <iosfwd>
namespace mge {

    /**
     * Usage of a buffer.
     */
    enum class MGE_GRAPHICS_EXPORT buffer_usage
    {
        DEFAULT     = 1,      //!< default usage, no specific restrictions
        IMMUTABLE   = 2,      //!< buffer is initialized once, and never changed
        DYNAMIC     = 3,      //!< buffer will change occasionally
        STREAMING   = 4,      //!< buffer will change frequently
    };

    MGE_GRAPHICS_EXPORT std::ostream& operator <<(std::ostream& os, const buffer_usage& u);
}
