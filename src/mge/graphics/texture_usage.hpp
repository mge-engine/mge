// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"

namespace mge {

    /**
     * @brief Intended usage of a texture.
     *
     * Backends use this at creation time to select the appropriate memory
     * type, bind flags, and image usage bits.
     */
    enum class MGEGRAPHICS_EXPORT texture_usage
    {
        SAMPLE,        //!< CPU-uploaded texture sampled in shaders (default)
        RENDER_TARGET  //!< GPU-written color attachment for render passes
    };

} // namespace mge
