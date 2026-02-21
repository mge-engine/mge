// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/input_handler.hpp"

namespace mge {
    input_handler::input_handler() {}

    input_handler::~input_handler() {}

    input_handler::key_type
    input_handler::add_mouse_action_handler(const mouse_action_handler& handler)
    {
        key_type key = ++m_next_key;
        m_mouse_action_handlers[key] = handler;
        return key;
    }

    input_handler::key_type
    input_handler::add_mouse_move_handler(const mouse_move_handler& handler)
    {
        key_type key = ++m_next_key;
        m_mouse_move_handlers[key] = handler;
        return key;
    }

    input_handler::key_type
    input_handler::add_mouse_wheel_handler(const mouse_wheel_handler& handler)
    {
        key_type key = ++m_next_key;
        m_mouse_wheel_handlers[key] = handler;
        return key;
    }

    input_handler::key_type
    input_handler::add_key_action_handler(const key_action_handler& handler)
    {
        key_type key = ++m_next_key;
        m_key_action_handlers[key] = handler;
        return key;
    }

    input_handler::key_type
    input_handler::add_character_handler(const character_handler& handler)
    {
        key_type key = ++m_next_key;
        m_character_handlers[key] = handler;
        return key;
    }

    void input_handler::remove_mouse_action_handler(key_type key)
    {
        m_mouse_action_handlers.erase(key);
    }

    void input_handler::remove_mouse_move_handler(key_type key)
    {
        m_mouse_move_handlers.erase(key);
    }

    void input_handler::remove_mouse_wheel_handler(key_type key)
    {
        m_mouse_wheel_handlers.erase(key);
    }

    void input_handler::remove_key_action_handler(key_type key)
    {
        m_key_action_handlers.erase(key);
    }

    void input_handler::remove_character_handler(key_type key)
    {
        m_character_handlers.erase(key);
    }

    void input_handler::clear_mouse_action_handler()
    {
        m_mouse_action_handlers.clear();
    }

    void input_handler::clear_mouse_move_handler()
    {
        m_mouse_move_handlers.clear();
    }

    void input_handler::clear_mouse_wheel_handler()
    {
        m_mouse_wheel_handlers.clear();
    }

    void input_handler::clear_key_action_handler()
    {
        m_key_action_handlers.clear();
    }

    void input_handler::clear_character_handler()
    {
        m_character_handlers.clear();
    }

    bool input_handler::on_mouse_action(uint32_t     button,
                                        mouse_action action,
                                        uint32_t     x,
                                        uint32_t     y)
    {
        m_input_state.mouse().move(x, y);
        if (action == mouse_action::PRESS) {
            m_input_state.mouse().press(button);
        } else {
            m_input_state.mouse().release(button);
        }
        for (const auto& [key, handler] : m_mouse_action_handlers) {
            if (handler(button, action, current_modifier(), x, y)) {
                return true;
            }
        }
        return false;
    }

    bool input_handler::on_mouse_move(uint32_t x, uint32_t y)
    {
        m_input_state.mouse().move(x, y);
        for (const auto& [key, handler] : m_mouse_move_handlers) {
            if (handler(x, y)) {
                return true;
            }
        }
        return false;
    }

    bool input_handler::on_mouse_wheel(int32_t x, int32_t y)
    {
        m_input_state.mouse().wheel(x, y);
        for (const auto& [key, handler] : m_mouse_wheel_handlers) {
            if (handler(x, y)) {
                return true;
            }
        }
        return false;
    }

    bool input_handler::on_character(uint32_t ch)
    {
        for (const auto& [key, handler] : m_character_handlers) {
            if (handler(ch)) {
                return true;
            }
        }
        return false;
    }

    bool input_handler::on_key_action(key k, key_action action)
    {
        update_key_state(k, action);
        for (const auto& [key, handler] : m_key_action_handlers) {
            if (handler(k, action, current_modifier())) {
                return true;
            }
        }
        return false;
    }

    const modifier& input_handler::current_modifier() const
    {
        return m_input_state.keyboard().current_modifier();
    }

    const input_state& input_handler::state() const
    {
        return m_input_state;
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
