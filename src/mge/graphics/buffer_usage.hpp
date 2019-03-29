#pragma once
#include "mge/graphics/dllexport.hpp"
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
}