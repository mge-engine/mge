// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/gamma_ramp.hpp"
#include <cmath>

namespace mge {
    gamma_ramp::gamma_ramp() : m_size(256)
    {
        m_red.resize(256);

        for (size_t i = 0; i < 256; ++i) {
            double value;
            value = i / (255.0);
            value = value * 65535.0 + 0.5;
            if (value > 65535.0)
                value = 65535.0;
            m_red[i] = (uint16_t)value;
        }

        m_green = m_red;
        m_blue  = m_red;
    }

    gamma_ramp::gamma_ramp(float gamma, size_t size) : m_size(size)
    {
        m_red.resize(size);

        for (size_t i = 0; i < size; ++i) {
            double value;
            value = i / (double)(size - 1);
            value = pow(value, 1.0 / gamma) * 65535.0 + 0.5;
            if (value > 65535.0)
                value = 65535.0;
            m_red[i] = (uint16_t)value;
        }

        m_green = m_red;
        m_blue  = m_red;
    }

    gamma_ramp::gamma_ramp(const gamma_ramp &ramp)
        : m_size(ramp.m_size), m_red(ramp.m_red), m_green(ramp.m_green),
          m_blue(ramp.m_blue)
    {}

    gamma_ramp::gamma_ramp(gamma_ramp &&ramp)
        : m_size(ramp.m_size), m_red(std::move(ramp.m_red)),
          m_green(std::move(ramp.m_green)), m_blue(std::move(ramp.m_blue))
    {}

    gamma_ramp &gamma_ramp::operator=(const gamma_ramp &ramp)
    {
        m_size  = ramp.m_size;
        m_red   = ramp.m_red;
        m_green = ramp.m_green;
        m_blue  = ramp.m_blue;
        return *this;
    }

    size_t gamma_ramp::size() const { return m_size; }

    const uint16_t *gamma_ramp::red_ramp() const { return &m_red[0]; }

    const uint16_t *gamma_ramp::green_ramp() const { return &m_green[0]; }

    const uint16_t *gamma_ramp::blue_ramp() const { return &m_blue[0]; }

    uint16_t *gamma_ramp::red_ramp() { return &m_red[0]; }

    uint16_t *gamma_ramp::green_ramp() { return &m_green[0]; }

    uint16_t *gamma_ramp::blue_ramp() { return &m_blue[0]; }

} // namespace mge
