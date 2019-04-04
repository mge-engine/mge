#pragma once
#include "mge/math/dllexport.hpp"
#include "mge/math/vec3.hpp"
#include <iosfwd>

namespace mge {
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