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
         * Handler function for mouse actions.
         */
        using mouse_action_handler =
            std::function<void(uint32_t button, mouse_action action,
                               const modifier &m, uint32_t x, uint32_t y)>;
        /**
         * Handler function for mouse movement.
         */
        using mouse_move_handler = std::function<void(uint32_t x, uint32_t y)>;

        /**
         * Handler function for keyboard actions.
         */
        using key_action_handler =
            std::function<void(key k, key_action action, const modifier &m)>;

        /**
         * Handler function for character input.
         */
        using character_handler = std::function<void(uint32_t character)>;

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

        const input_state &state() const;
        const modifier &   current_modifier() const;

    protected:
        void on_mouse_action(uint32_t button, mouse_action action, uint32_t x,
                             uint32_t y);
        void on_key_action(key k, key_action action);
        void on_mouse_move(uint32_t x, uint32_t y);
        void on_character(uint32_t ch);

    private:
        void update_key_state(key k, key_action action);

        mouse_action_handler m_mouse_action_handler;
        mouse_move_handler   m_mouse_move_handler;
        key_action_handler   m_key_action_handler;
        character_handler    m_character_handler;

    protected:
        input_state m_input_state;
    };
} // namespace mge
