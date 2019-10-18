// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/scene/scene_fwd.hpp"
namespace mge {

    class MGE_SCENE_EXPORT renderer
    {
    public:
        renderer();
        ~renderer();
        void prepare(scene& s);
        void render(scene& s);
    };
}