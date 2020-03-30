// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
namespace mge {

    /**
     * @brief Draw mode, defines how vertices shall be interpreted.
     */
    enum class topology
    {
        TRIANGLES,
        TRIANGLE_STRIP,
        LINES,
        LINE_STRIP
    };
} // namespace mge
