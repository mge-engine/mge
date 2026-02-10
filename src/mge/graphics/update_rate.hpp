// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/graphics/dllexport.hpp"

namespace mge {

    /** @brief Update rate of uniforms when drawing.
     *
     * This is used to specify how frequently uniform values are updated when
     * drawing with a program. For example, some uniforms may be updated
     * per-frame, while others may be updated per-draw or per-instance. The
     * update rate can be used by the render context to optimize uniform updates
     * and resource bindings.
     *
     * "When changed" is a special update rate that indicates that the uniform
     * should only be updated when its value changes. This can be used for
     * uniforms that rarely change like material properties, to avoid
     * unnecessary updates.
     */
    enum class update_rate
    {
        WHEN_CHANGED, //!< Uniforms are updated only when their values change.
        PER_FRAME,    //!< Uniforms are updated once per frame.
        PER_PASS,     //!< Uniforms are updated once per render pass.
        PER_DRAW,     //!< Uniforms are updated for each draw call.
        PER_INSTANCE  //!< Uniforms are updated for each instance in instanced
                      //!< drawing.
    };
} // namespace mge