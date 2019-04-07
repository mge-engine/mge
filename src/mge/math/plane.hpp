// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/dllexport.hpp"
#include "mge/math/vec3.hpp"
#include <iosfwd>

namespace mge {
    /**
     * A plane. A plane is defined by a normal vector \f$\vec{n}\f$ and a constant
     * value \f$c\f$ such that the formulat \f$\vec{n} \cdot \vec{p} = c\f$
     * is fullfilled for \f$p\f$ being a point on the plane (displacement).
     */
    class MGE_MATH_EXPORT plane
    {
    public:
        plane();
        plane(const fvec3& normal, float constant);
        plane(const fvec3& normal, const fvec3& displacement);
        plane(const fvec3& a, const fvec3& b, const fvec3& c);
        plane(const plane& p);
        plane& operator =(const plane& p);

        inline fvec3 normal() const noexcept { return m_normal; }
        inline float constant() const noexcept { return m_constant; }
    private:
        fvec3 m_normal;
        float m_constant;
    };

    MGE_MATH_EXPORT std::ostream& operator << (const std::ostream& os, const plane& p);
}