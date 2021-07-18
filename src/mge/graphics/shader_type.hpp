// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <iosfwd>
namespace mge {

    /**
     * Type of shader.
     */
    enum class MGEGRAPHICS_EXPORT shader_type : uint8_t
    {
        VERTEX                 = 0, //!< vertex
        FRAGMENT               = 1, //!< fragment (pixel)
        COMPUTE                = 2, //!< compute
        TESSELATION_CONTROL    = 3, //!< tesselation control (hull)
        TESSELATION_EVALUATION = 4, //!< tesselation evaluation (domain)
        GEOMETRY               = 5, //!< geometry
        MAX_SHADER_TYPE        = GEOMETRY
    };

    MGEGRAPHICS_EXPORT std::ostream &operator<<(std::ostream &     os,
                                                const shader_type &t);
} // namespace mge
