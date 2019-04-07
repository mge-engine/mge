// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/dllexport.hpp"
#include "mge/math/vec3.hpp"

namespace mge {
    class MGE_MATH_EXPORT ray
    {
    public:
        ray()
            :m_origin(0.0f, 0.0f, 0.0f)
            ,m_direction(1.0f, 0.0f, 0.0f)
        {}

        ray(const fvec3& origin, const fvec3& direction)
            :m_origin(origin)
            ,m_direction(direction)
        {}

        ray(const ray& r)
            :m_origin(r.m_origin)
            ,m_direction(r.m_direction)
        {}

        fvec3 origin() const noexcept { return m_origin; }
        fvec3 direction() const noexcept { return m_direction; }
    private:
        fvec3 m_origin;
        fvec3 m_direction;
    };

    MGE_MATH_EXPORT std::ostream& operator <<(std::ostream& os, const ray& r);
}
