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
    enum class MGE_GRAPHICS_EXPORT buffer_type
    {
        VERTEX_BUFFER   = 0x01,
        INDEX_BUFFER    = 0x02,
        CONSTANT_BUFFER = 0x04
    };

    MGE_GRAPHICS_EXPORT std::ostream &operator<<(std::ostream &     os,
                                                 const buffer_type &t);

} // namespace mge