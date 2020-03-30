// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/dllexport.hpp"
#include "mge/math/vec3.hpp"
#include <iosfwd>

namespace mge {
    class MGE_MATH_EXPORT sphere
    {
    public:
        sphere() : m_center(0.0f, 0.0f, 0.0f), m_radius(0.0f) {}

        sphere(const fvec3 &center, float radius)
            : m_center(center), m_radius(radius)
        {}

        sphere(const sphere &s) : m_center(s.center()), m_radius(s.radius()) {}

        sphere &operator=(const sphere &s)
        {
            m_center = s.center();
            m_radius = s.radius();
            return *this;
        }

        fvec3 center() const noexcept { return m_center; }
        float radius() const noexcept { return m_radius; }

    private:
        fvec3 m_center;
        float m_radius;
    };

    MGE_MATH_EXPORT std::ostream &operator<<(std::ostream &os, const sphere &s);
} // namespace mge
