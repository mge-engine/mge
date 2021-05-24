// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/input/input_handler.hpp"

namespace mge {
    input_handler::input_handler() {}

    input_handler::~input_handler() {}

    void
    input_handler::set_mouse_action_handler(const mouse_action_handler &handler)
    {
        m_mouse_action_handler = handler;
    }

    void
    input_handler::set_mouse_move_handler(const mouse_move_handler &handler)
    {
        m_mouse_move_handler = handler;
    }

    void
    input_handler::set_key_action_handler(const key_action_handler &handler)
    {
        m_key_action_handler = handler;
    }

    void input_handler::set_character_handler(const character_handler &handler)
    {
        m_character_handler = handler;
    }

    void input_handler::clear_mouse_action_handler()
    {
        m_mouse_action_handler = nullptr;
    }

    void input_handler::clear_mouse_move_handler()
    {
        m_mouse_move_handler = nullptr;
    }

    void input_handler::clear_key_action_handler()
    {
        m_key_action_handler = nullptr;
    }

    void input_handler::clear_character_handler()
    {
        m_character_handler = nullptr;
    }

    void input_handler::on_mouse_action(uint32_t button, mouse_action action,
                                        uint32_t x, uint32_t y)
    {
        m_input_state.mouse().move(x, y);
        if (action == mouse_action::PRESS) {
            m_input_state.mouse().press(button);
        } else {
            m_input_state.mouse().release(button);
        }
        if (m_mouse_action_handler) {
            m_mouse_action_handler(button, action, current_modifier(), x, y);
        }
    }

    void input_handler::on_mouse_move(uint32_t x, uint32_t y)
    {
        m_input_state.mouse().move(x, y);
        if (m_mouse_move_handler) {
            m_mouse_move_handler(x, y);
        }
    }

    void input_handler::on_character(uint32_t ch)
    {
        if (m_character_handler) {
            m_character_handler(ch);
        }
    }

    void input_handler::on_key_action(key k, key_action action)
    {
        update_key_state(k, action);
        if (m_key_action_handler) {
            m_key_action_handler(k, action, current_modifier());
        }
    }

    const modifier &input_handler::current_modifier() const
    {
        return m_input_state.keyboard().current_modifier();
    }

    void input_handler::update_key_state(key k, key_action action)
    {
        if (action == key_action::PRESS) {
            m_input_state.keyboard().press(k);
        } else if (action == key_action::RELEASE) {
            m_input_state.keyboard().release(k);
        }
    }

} // namespace mge
