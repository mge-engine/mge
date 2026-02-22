// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/callback_map.hpp"
#include "mge/input/dllexport.hpp"
#include "mge/input/input_state.hpp"
#include "mge/input/key.hpp"
#include "mge/input/key_action.hpp"
#include "mge/input/modifier.hpp"
#include "mge/input/mouse_action.hpp"

namespace mge {

    /**
     * Base class for objects handling user input. An object of this
     * class is provided with input device actions (the @c on_... methods)
     * dispatches them to handlers and also retains the current input state
     * (i.e. modifiers, keys that are pressed and mouse buttons
     * that are pressed).
     */
    class MGEINPUT_EXPORT input_handler
    {
    public:
        /**
         * Key type for handler registration.
         */
        using key_type = uint32_t;

        /**
         * Handler function for mouse actions.
         * @return true if event was consumed, false otherwise
         */
        using mouse_action_handler = std::function<bool(uint32_t        button,
                                                        mouse_action    action,
                                                        const modifier& m,
                                                        uint32_t        x,
                                                        uint32_t        y)>;
        /**
         * Handler function for mouse movement.
         * @return true if event was consumed, false otherwise
         */
        using mouse_move_handler = std::function<bool(uint32_t x, uint32_t y)>;

        /**
         * Handler function for keyboard actions.
         * @return true if event was consumed, false otherwise
         */
        using key_action_handler =
            std::function<bool(key k, key_action action, const modifier& m)>;

        /**
         * Handler function for character input.
         * @return true if event was consumed, false otherwise
         */
        using character_handler = std::function<bool(uint32_t character)>;

        /**
         * Handler function for mouse wheel input.
         * @param x horizontal wheel movement
         * @param y vertical wheel movement
         * @return true if event was consumed, false otherwise
         */
        using mouse_wheel_handler = std::function<bool(int32_t x, int32_t y)>;

    protected:
        /**
         * Constructor.
         */
        input_handler();

    public:
        input_handler(const input_handler&) = delete;
        input_handler& operator=(const input_handler&) = delete;

        /**
         * Destructor.
         */
        virtual ~input_handler();

        /**
         * Adds a handler for mouse actions.
         * @param handler mouse action handler
         * @return key for later removal
         */
        key_type add_mouse_action_handler(const mouse_action_handler& handler);
        /**
         * Adds a handler for mouse movement.
         * @param handler mouse movement handler
         * @return key for later removal
         */
        key_type add_mouse_move_handler(const mouse_move_handler& handler);

        /**
         * Adds a handler for mouse wheel input.
         * @param handler mouse wheel handler
         * @return key for later removal
         */
        key_type add_mouse_wheel_handler(const mouse_wheel_handler& handler);

        /**
         * Adds a handler for key actions.
         * @param handler key action handler
         * @return key for later removal
         */
        key_type add_key_action_handler(const key_action_handler& handler);

        /**
         * Adds a handler for character input.
         * @param handler character input handler
         * @return key for later removal
         */
        key_type add_character_handler(const character_handler& handler);

        /**
         * Removes a mouse action handler.
         * @param key handler key returned from add_mouse_action_handler
         */
        void remove_mouse_action_handler(key_type key);
        /**
         * Removes a mouse move handler.
         * @param key handler key returned from add_mouse_move_handler
         */
        void remove_mouse_move_handler(key_type key);
        /**
         * Removes a mouse wheel handler.
         * @param key handler key returned from add_mouse_wheel_handler
         */
        void remove_mouse_wheel_handler(key_type key);
        /**
         * Removes a key action handler.
         * @param key handler key returned from add_key_action_handler
         */
        void remove_key_action_handler(key_type key);
        /**
         * Removes a character handler.
         * @param key handler key returned from add_character_handler
         */
        void remove_character_handler(key_type key);

        /**
         * @brief Clears the mouse action handler.
         */
        void clear_mouse_action_handler();
        /**
         * @brief Clears the mouse move handler.
         */
        void clear_mouse_move_handler();
        /**
         * @brief Clears the mouse wheel handler.
         */
        void clear_mouse_wheel_handler();
        /**
         * @brief Clears the key action handler.
         */
        void clear_key_action_handler();
        /**
         * @brief Clears the character handler.
         */
        void clear_character_handler();

        /**
         * @brief Access input state.
         *
         * @return current input state
         */
        const input_state& state() const;

        /**
         * @brief Access the currently pressed modifier.
         *
         * @return current modifier
         */
        const modifier& current_modifier() const;

    protected:
        /**
         * @brief Method to be called when a mouse action is happening.
         *
         * @param button button of the action
         * @param action action
         * @param x mouse x position
         * @param y mouse y position
         * @return true if event was consumed, false otherwise
         */
        bool on_mouse_action(uint32_t     button,
                             mouse_action action,
                             uint32_t     x,
                             uint32_t     y);
        /**
         * @brief Method to be called when a key action happens.
         *
         * @param k       key of the action
         * @param action  key action
         * @return true if event was consumed, false otherwise
         */
        bool on_key_action(key k, key_action action);
        /**
         * @brief Method to be called on mouse move.
         *
         * @param x new mouse x position
         * @param y new mouse y position
         * @return true if event was consumed, false otherwise
         */
        bool on_mouse_move(uint32_t x, uint32_t y);

        /**
         * @brief Method to be called on mouse wheel movement.
         *
         * @param x horizontal wheel movement
         * @param y vertical wheel movement
         * @return true if event was consumed, false otherwise
         */
        bool on_mouse_wheel(int32_t x, int32_t y);

        /**
         * @brief Method to be called when a character is input.
         *
         * @param ch input character
         * @return true if event was consumed, false otherwise
         */
        bool on_character(uint32_t ch);

    private:
        void update_key_state(key k, key_action action);

        std::map<key_type, mouse_action_handler> m_mouse_action_handlers;
        std::map<key_type, mouse_move_handler>   m_mouse_move_handlers;
        std::map<key_type, mouse_wheel_handler>  m_mouse_wheel_handlers;
        std::map<key_type, key_action_handler>   m_key_action_handlers;
        std::map<key_type, character_handler>    m_character_handlers;
        key_type                                 m_next_key{0};

    protected:
        input_state m_input_state;
    };
} // namespace mge
