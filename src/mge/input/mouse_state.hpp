// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/input/dllexport.hpp"
#include <bitset>
#include <cstdint>
namespace mge {

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
        uint32_t x() const
        {
            return m_x;
        }
        /**
         * @brief Mouse y position.
         *
         * @return current y position of the mouse
         */
        uint32_t y() const
        {
            return m_y;
        }

        /**
         * @brief Mouse wheel x position.
         *
         * @return current x position of the mouse wheel
         */
        int32_t wheel_x() const
        {
            return m_wheel_x;
        }

        /**
         * @brief Mouse wheel y position.
         *
         * @return current y position of the mouse wheel
         */
        int32_t wheel_y() const
        {
            return m_wheel_y;
        }

        /**
         * @brief Returns whether a button is pressed.
         *
         * @param button button to check
         * @return @c true if currently pressed
         */
        bool pressed(uint32_t button) const
        {
            return m_buttons.test(button);
        }

        /**
         * @brief Signal mouse wheel movement.
         *
         * @param xdelta horizontal wheel movement
         * @param ydelta vertical wheel movement
         */
        void wheel(int32_t xdelta, int32_t ydelta);

    private:
        std::bitset<32> m_buttons;
        uint32_t        m_x{0};
        uint32_t        m_y{0};
        int32_t         m_wheel_x{0};
        int32_t         m_wheel_y{0};
    };
} // namespace mge
