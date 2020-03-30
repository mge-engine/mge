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
    enum class usage
    {
        DEFAULT   = 1, //!< default usage, no specific restrictions
        IMMUTABLE = 2, //!< resource is initialized once, and never changed
        DYNAMIC   = 3, //!< resource will change frequently
        STAGING   = 4, //!< resource will used to copy data to/frorm the GPU
    };

    /**
     * Stream output.
     * @param os    output stream
     * @param u     usage
     * @return @c os
     */
    MGE_GRAPHICS_EXPORT std::ostream &operator<<(std::ostream &os,
                                                 const usage & u);
} // namespace mge
