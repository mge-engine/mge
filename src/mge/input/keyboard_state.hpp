// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/input/dllexport.hpp"
#include "mge/input/key.hpp"
#include "mge/input/modifier.hpp"

namespace mge {
    /**
     * Input state of a keyboard.
     */
    class MGEINPUT_EXPORT keyboard_state
    {
    public:
        /**
         * @brief Construct empty keyboard state.
         */
        keyboard_state();

        /**
         * @brief Destructor
         */
        ~keyboard_state();

        /**
         * @brief Signal press of modifier key.
         * @param m modifier
         */
        void press(const modifier &m);
        /**
         * @brief Signal press of key.
         * @param k key
         */
        void press(const key &k);
        /**
         * @brief Release modifier key.
         * @param m modifier key
         */
        void release(const modifier &m);

        /**
         * @brief Release key
         * @param k key
         */
        void release(const key &k);

        /**
         * @brief Retrieve whether key is pressed.
         * @param k key
         * @return @c true if pressed
         */
        bool pressed(const key &k) const;

        /**
         * @brief Retrieve whether key is pressed.
         * @param m modifier key
         * @return @c true if pressed
         */
        bool pressed(const modifier &m) const;

        /**
         * @brief Retrieve whether modifier/key combination is pressed.
         * @param m modfier key
         * @param k key
         * @return @c true if pressed
         */
        bool pressed(const modifier &m, const key &k) const;

        /**
         * @brief Get current modifier state.
         *
         * @return modifier state
         */
        const mge::modifier &current_modifier() const;

    private:
        mge::modifier     m_modifier;
        std::vector<bool> m_pressed;
        bool              m_invalid_pressed;
    };

} // namespace mge
