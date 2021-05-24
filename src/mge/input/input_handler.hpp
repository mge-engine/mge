#pragma once

#include "mge/core/callback_map.hpp"
#include "mge/input/dllexport.hpp"
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
    class MGE_INPUT_EXPORT input_handler
    {
    public:
        /**
         * Handler function for mouse actions.
         */
        using mouse_action_handler = std::function<void(
            int button, mouse_action action, modifier m, int x, int y)>;
        /**
         * Handler function for mouse movement.
         */
        using mouse_move_handler = std::function<void(int x, int y)>;

        /**
         * Handler function for keyboard actions.
         */
        using key_action_handler =
            std::function<void(key k, key_action action, modifier m)>;

        /**
         * Handler function for character input.
         */
        using character_handler = std::function<void(unsigned int character)>;

    protected:
        /**
         * Constructor.
         */
        input_handler();

    public:
        /**
         * Destructor.
         */
        virtual ~input_handler();

        /**
         * Sets the handler used to dispatch mouse actions.
         * @param handler mouse action handler
         */
        void set_mouse_action_handler(const mouse_action_handler &handler);
        /**
         * Sets the handler used to dispatch mouse movement.
         * @param handler mouse movement handler
         */
        void set_mouse_move_handler(const mouse_move_handler &handler);

        /**
         * Sets the handler used to dispatch key actions.
         * @param handler key action handler
         */
        void set_key_action_handler(const key_action_handler &handler);

        /**
         * Sets the handler used to dispatch character input.
         * @param handler character input handler
         */
        void set_character_handler(const character_handler &handler);

        void clear_mouse_action_handler();
        void clear_mouse_move_handler();
        void clear_key_action_handler();
        void clear_character_handler();

        /**
         * Return whether a certain key is pressed.
         * @param k key to check
         * @return bool if key is currently pressed
         */
        bool key_down(key k) const;

        /**
         * Returns current modifier state.
         * @return modifier state
         */
        const modifier &current_modifier();

    protected:
        void on_mouse_action(int button, mouse_action action, int x, int y);
        void on_key_action(key k, key_action action);
        void on_mouse_move(int x, int y);
        void on_character(unsigned int ch);
        void add_modifier(modifier_value m);
        void remove_modifier(modifier_value m);

    private:
        void update_key_state(key k, key_action action);

        modifier                       m_current_modifier;
        mouse_action_handler           m_mouse_action_handler;
        mouse_move_handler             m_mouse_move_handler;
        key_action_handler             m_key_action_handler;
        character_handler              m_character_handler;
        std::bitset<(int)key::KEY_MAX> m_pressed_keys;
    };
} // namespace mge
