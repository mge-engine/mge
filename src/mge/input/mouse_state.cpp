// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/mouse_state.hpp"

namespace mge {
    mouse_state::mouse_state()
        : m_x(0)
        , m_y(0)
    {}

    void mouse_state::press(uint32_t button)
    {
        m_buttons.set(button);
    }

    void mouse_state::release(uint32_t button)
    {
        m_buttons.reset(button);
    }

    void mouse_state::move(uint32_t x, uint32_t y)
    {
        m_x = x;
        m_y = y;
    }

    void mouse_state::wheel(int32_t xdelta, int32_t ydelta)
    {
        m_wheel_x += xdelta;
        m_wheel_y += ydelta;
    }
} // namespace mge
