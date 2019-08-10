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
    enum class MGE_GRAPHICS_EXPORT buffer_change_policy
    {
        DEFAULT     = 1,      //!< default usage, no specific restrictions
        IMMUTABLE   = 2,      //!< buffer is initialized once, and never changed
        DYNAMIC     = 3,      //!< buffer will change frequently
        STAGING     = 4,      //!< buffer will used to copy data to the GPU
    };

    MGE_GRAPHICS_EXPORT std::ostream& operator <<(std::ostream& os, const buffer_change_policy& p);
}
