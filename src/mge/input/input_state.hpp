// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/input/dllexport.hpp"
#include "mge/input/keyboard_state.hpp"
#include "mge/input/mouse_state.hpp"

namespace mge {
    /**
     * @brief Input state.
     * An input states describes the current state - pressed buttons and keys -
     * of input devices.
     */
    class MGEINPUT_EXPORT input_state
    {
    public:
        /**
         * Constructor.
         */
        input_state();
        /**
         * Destructor.
         */
        ~input_state();

        /**
         * @brief keyboard state
         * @return current keyboard state
         */
        keyboard_state& keyboard();

        /**
         * @brief keyboard state
         * @return current keyboard state
         */
        const keyboard_state& keyboard() const;

        /**
         * @brief mouse state
         * @return current mouse state
         */
        mouse_state& mouse();

        /**
         * @brief mouse state
         * @return current mouse state
         */
        const mouse_state& mouse() const;

    private:
        keyboard_state m_keyboard;
        mouse_state    m_mouse;
    };

} // namespace mge
