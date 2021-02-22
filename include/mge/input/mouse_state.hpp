// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/input/dllexport.hpp"
#include <bitset>
namespace mge {
    class MGEINPUT_EXPORT mouse_state
    {
    public:
        mouse_state();
        ~mouse_state();

        void press(uint32_t button);
        void release(uint32_t button);
        void move(uint32_t x, uint32_t y);

        uint32_t x() const { return m_x; }
        uint32_t y() const { return m_y; }

        bool pressed(uint32_t button) const { return m_buttons.test(button); }

    private:
        std::bitset<32> m_buttons;
        uint32_t        m_x;
        uint32_t        m_y;
    };
} // namespace mge
