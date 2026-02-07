// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/graphics/dllexport.hpp"

namespace mge {

    /**
     * @brief Blend operations define how source and destination colors are
     * combined.
     *
     * The blend equation combines source and destination colors using the
     * specified operation and blend factors.
     */
    enum class blend_operation : uint8_t
    {
        NONE, //!< No blending, source color is used directly
        ADD,  //!< Add source and destination: src * srcFactor + dst * dstFactor
        SUBTRACT, //!< Subtract destination from source: src * srcFactor - dst *
                  //!< dstFactor
        REVERSE_SUBTRACT, //!< Subtract source from destination: dst * dstFactor
                          //!< - src * srcFactor
        MIN,              //!< Minimum of source and destination: min(src, dst)
        MAX               //!< Maximum of source and destination: max(src, dst)
    };
} // namespace mge
