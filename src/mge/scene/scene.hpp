// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/scene/dllexport.hpp"

namespace mge {

    /**
     * @brief Scene.
     */
    class MGESCENE_EXPORT scene
    {
    public:
        scene() = default;
        scene(const scene&) = default;
        scene(scene&&) = default;
        scene& operator=(const scene&) = default;
        scene& operator=(scene&&) = default;
    };

} // namespace mge
