// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/input/dllexport.hpp"
#include <bitset>
namespace mge {

    // TODO: Mouse wheel support
    /**
     * @brief Current state of mouse input.
     *
     */
    class MGEINPUT_EXPORT mouse_state
    {
    public:
        /**
         * @brief Constructor.
         */
        mouse_state();

        /**
         * @brief Destructor.
         */
        ~mouse_state() = default;

        mouse_state(const mouse_state&) = default;
        mouse_state& operator=(const mouse_state&) = default;

        /**
         * @brief Signal press of button.
         *
         * @param button pressed button
         */
        void press(uint32_t button);

        /**
         * @brief Signal release of button.
         *
         * @param button released button
         */
        void release(uint32_t button);

        /**
         * @brief Signal mouse move.
         *
         * @param x new x position
         * @param y new y position
         */
        void move(uint32_t x, uint32_t y);

        /**
         * @brief Mouse x position.
         *
         * @return current x position of the mouse
         */
        uint32_t x() const { return m_x; }
        /**
         * @brief Mouse y position.
         *
         * @return current y position of the mouse
         */
        uint32_t y() const { return m_y; }

        /**
         * @brief Returns whether a button is pressed.
         *
         * @param button button to check
         * @return @c true if currently pressed
         */
        bool pressed(uint32_t button) const { return m_buttons.test(button); }

    private:
        std::bitset<32> m_buttons;
        uint32_t        m_x{0};
        uint32_t        m_y{0};
    };
} // namespace mge
