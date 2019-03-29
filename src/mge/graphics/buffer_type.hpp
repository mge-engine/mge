#pragma once
#include "mge/graphics/dllexport.hpp"
namespace mge {
    
    /**
     * Usage of a buffer.
     */
    enum class MGE_GRAPHICS_EXPORT buffer_type
    {
        VERTEX_BUFFER       = 0x01,
        INDEX_BUFFER        = 0x02,
        CONSTANT_BUFFER     = 0x04
    };
}