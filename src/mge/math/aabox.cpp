// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/math/aabox.hpp"
#include <iostream>
#include <algorithm>

namespace mge {

    aabox::aabox()
        :m_min(0.0f, 0.0f, 0.0f)
        ,m_max(0.0f, 0.0f, 0.0f)
    {}


    aabox::aabox(const fvec3& corner1, const fvec3& corner2)
    {
        m_min.x = std::min(corner1.x, corner2.x);
        m_min.y = std::min(corner1.y, corner2.y);
        m_min.z = std::min(corner1.z, corner2.z);

        m_max.x = std::max(corner1.x, corner2.x);
        m_max.y = std::max(corner1.y, corner2.y);
        m_max.z = std::max(corner1.z, corner2.z);
    }

    aabox::aabox(const aabox& b)
        :m_min(b.m_min)
        ,m_max(b.m_max)
    {}

    aabox&
    aabox::operator =(const aabox& b)
    {
        m_min = b.m_min;
        m_max = b.m_max;
        return *this;
    }

    bool
    aabox::contains(const fvec3& v) const
    {
        return v == clamp(v, m_min, m_max);
    }

    std::ostream& operator <<(std::ostream& os, const aabox& b)
    {
        return os << "aabox[min=" << b.min() << ", max=" << b.max() << "]";
    }
}