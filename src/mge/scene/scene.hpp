// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/scene/dllexport.hpp"

namespace mge {
    /**
     * A scene combines all objects displayed and interacting
     * with each other (and can be interacted with).
     */
    class MGE_SCENE_EXPORT scene
    {
    public:
        scene();
        virtual ~scene();
    private:

    };
}
