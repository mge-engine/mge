/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#pragma once

#include "mge/graphics/dllexport.hpp"

#include <vector>
#include <cstddef>
#include <cstdint>

namespace mge {

    class MGE_GRAPHICS_EXPORT gamma_ramp
    {
    public:
        gamma_ramp();
        gamma_ramp(float gamma, size_t size=256);
        gamma_ramp(const gamma_ramp& ramp);
        gamma_ramp(gamma_ramp&& ramp);
        ~gamma_ramp() = default;

        gamma_ramp& operator =(const gamma_ramp& ramp);
        size_t size() const;

        const uint16_t *red_ramp() const;
        const uint16_t *green_ramp() const;
        const uint16_t *blue_ramp() const;

        uint16_t *red_ramp();
        uint16_t *green_ramp();
        uint16_t *blue_ramp();

    private:
        size_t m_size;
        std::vector<uint16_t> m_red;
        std::vector<uint16_t> m_green;
        std::vector<uint16_t> m_blue;
    };
}
