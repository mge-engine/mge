// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/scene/dllexport.hpp"

namespace mge {

    /**
     * Type of material.
     */
    enum class MGESCENE_EXPORT material_type
    {
        TEXTURE, //!< Simple texture material
        PBR,     //!< Physically-based rendering material
        SHADER   //!< Custom shader material
    };

} // namespace mge
