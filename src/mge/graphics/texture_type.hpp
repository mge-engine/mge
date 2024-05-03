// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/graphics/dllexport.hpp"
#include <format>
#include <iosfwd>

namespace mge {
    /**
     * Type of texture.
     */
    enum class MGEGRAPHICS_EXPORT texture_type
    {
        TYPE_1D,      //!< 1D texture
        TYPE_2D,      //!< 2D texture
        TYPE_3D,      //!< 3D texture
        TYPE_CUBE_MAP //!< Cube map texture
    };

} // namespace mge
