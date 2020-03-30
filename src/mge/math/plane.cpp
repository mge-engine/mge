// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/math/plane.hpp"
#include <iostream>

namespace mge {
    plane::plane() : m_normal(0.0f, 0.0f, 0.0f), m_constant(0.0f) {}

    plane::plane(const fvec3 &normal, float constant)
        : m_normal(normal), m_constant(constant)
    {}

    plane::plane(const fvec3 &normal, const fvec3 &displacement)
        : m_normal(normal), m_constant(mge::dot(displacement, normal))
    {}

    plane::plane(const fvec3 &a, const fvec3 &b, const fvec3 &c)
        : m_normal(0.0f, 0.0f, 0.0f), m_constant(0.0f)
    {
        auto e1    = b - a;
        auto e2    = c - a;
        m_normal   = mge::normalize(mge::cross(e1, e2));
        m_constant = mge::dot(m_normal, a);
    }

    plane::plane(const plane &p)
        : m_normal(p.m_normal), m_constant(p.m_constant)
    {}

    plane &plane::operator=(const plane &p)
    {
        m_normal   = p.normal();
        m_constant = p.constant();
        return *this;
    }

    std::ostream &operator<<(std::ostream &os, const plane &p)
    {
        return os << "plane[normal=" << p.normal()
                  << ", constant=" << p.constant() << "]";
    }
} // namespace mge
