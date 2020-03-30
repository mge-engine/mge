// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"

namespace mge {

    /**
     * @brief Video mode as settable to a monitor.
     */
    class MGE_GRAPHICS_EXPORT video_mode
    {
    public:
        unsigned int width;
        unsigned int height;
        unsigned int refresh_rate;
    };
} // namespace mge
