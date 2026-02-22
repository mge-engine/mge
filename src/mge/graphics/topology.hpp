// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/graphics/dllexport.hpp"

namespace mge {

    /**
     * @brief Define how vertices are interpreted in drawing.
     */
    enum class topology : uint8_t
    {
        TRIANGLE_LIST,
        TRIANGLES = TRIANGLE_LIST,
        TRIANGLE_STRIP,
        LINES,
        LINE_LIST = LINES,
        POINTS,
        POINT_LIST = POINTS
    };
} // namespace mge
