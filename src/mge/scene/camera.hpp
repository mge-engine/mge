// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/math/vec3.hpp"

namespace mge {
    
    class MGE_SCENE_EXPORT camera
    {
    public:
        float m_aspect;
        float m_near_plane;
        float m_far_plane;
        float m_fov;
        fvec3 m_look_at;
        fvec3 m_position;
        fvec3 m_up;
    };

}