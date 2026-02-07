// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/graphics/dllexport.hpp"

namespace mge {

    /**
     * @brief Blend factors for source and destination colors.
     *
     * Blend factors determine how source and destination colors are weighted
     * in the blending equation.
     */
    enum class blend_factor
    {
        ZERO,                     //!< Factor is (0, 0, 0, 0)
        ONE,                      //!< Factor is (1, 1, 1, 1)
        SRC_COLOR,                //!< Factor is source color
        ONE_MINUS_SRC_COLOR,      //!< Factor is (1 - source color)
        DST_COLOR,                //!< Factor is destination color
        ONE_MINUS_DST_COLOR,      //!< Factor is (1 - destination color)
        SRC_ALPHA,                //!< Factor is source alpha
        ONE_MINUS_SRC_ALPHA,      //!< Factor is (1 - source alpha)
        DST_ALPHA,                //!< Factor is destination alpha
        ONE_MINUS_DST_ALPHA,      //!< Factor is (1 - destination alpha)
        CONSTANT_COLOR,           //!< Factor is constant color
        ONE_MINUS_CONSTANT_COLOR, //!< Factor is (1 - constant color)
        CONSTANT_ALPHA,           //!< Factor is constant alpha
        ONE_MINUS_CONSTANT_ALPHA, //!< Factor is (1 - constant alpha)
        SRC_ALPHA_SATURATE, //!< Factor is min(source alpha, 1 - destination
                            //!< alpha)
        SRC1_COLOR, //!< Factor is secondary source color (dual source blending)
        ONE_MINUS_SRC1_COLOR, //!< Factor is (1 - secondary source color)
        SRC1_ALPHA, //!< Factor is secondary source alpha (dual source blending)
        ONE_MINUS_SRC1_ALPHA //!< Factor is (1 - secondary source alpha)
    };
} // namespace mge
