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
    enum class topology
    {
        TRIANGLES,
        TRIANGLE_STRIP,
        LINES,
        LINE_STRIP
    };
} // namespace mge
