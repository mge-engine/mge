// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/graphics/dllexport.hpp"

namespace mge {

    /**
     * @brief Face culling modes for rasterization.
     *
     * Determines which triangle faces are culled (discarded) during
     * rasterization based on their winding order.
     */
    enum class cull_mode
    {
        NONE,             //!< No face culling
        CLOCKWISE,        //!< Cull clockwise faces
        COUNTER_CLOCKWISE //!< Cull counter-clockwise faces
    };
} // namespace mge
