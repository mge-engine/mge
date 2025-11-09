// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/keyboard_state.hpp"

namespace mge {
    keyboard_state::keyboard_state()
    {
        const uint32_t key_max = static_cast<uint32_t>(key::KEY_MAX);
        m_pressed.resize(key_max);
    }

    void keyboard_state::press(const modifier& m)
    {
        m_modifier |= m;
    }

    void keyboard_state::release(const modifier& m)
    {
        m_modifier.reset(m);
    }

    void keyboard_state::press(const key& k)
    {
        switch (k) {
        case key::LEFT_SHIFT:
        case key::RIGHT_SHIFT:
            m_modifier.set(modifier_value::SHIFT);
            break;
        case key::LEFT_ALT:
        case key::RIGHT_ALT:
            m_modifier.set(modifier_value::ALT);
            break;
        case key::LEFT_CONTROL:
        case key::RIGHT_CONTROL:
            m_modifier.set(modifier_value::CONTROL);
            break;
        default:
            break;
        }
        if (k != key::INVALID) {
            uint32_t pos = static_cast<uint32_t>(k);
            m_pressed[pos] = true;

        } else {
            m_invalid_pressed = true;
        }
    }

    void keyboard_state::release(const key& k)
    {
        switch (k) {
        case key::LEFT_SHIFT:
        case key::RIGHT_SHIFT:
            m_modifier.flip(modifier_value::SHIFT);
            break;
        case key::LEFT_ALT:
        case key::RIGHT_ALT:
            m_modifier.flip(modifier_value::ALT);
            break;
        case key::LEFT_CONTROL:
        case key::RIGHT_CONTROL:
            m_modifier.flip(modifier_value::CONTROL);
            break;
        default:
            break;
        }

        if (k != key::INVALID) {
            uint32_t pos = static_cast<uint32_t>(k);
            m_pressed[pos] = false;
        } else {
            m_invalid_pressed = false;
        }
    }

    bool keyboard_state::pressed(const key& k) const
    {
        if (k != key::INVALID) {
            uint32_t pos = static_cast<uint32_t>(k);
            return m_pressed[pos];
        } else {
            return m_invalid_pressed;
        }
    }

    bool keyboard_state::pressed(const modifier& m) const
    {
        return m_modifier.includes(m);
    }

    const mge::modifier& keyboard_state::current_modifier() const
    {
        return m_modifier;
    }

    bool keyboard_state::pressed(const modifier& m, const key& k) const
    {
        if (m_modifier.includes(m)) {
            if (k != key::INVALID) {
                uint32_t pos = static_cast<uint32_t>(k);
                return m_pressed[pos];
            } else {
                return m_invalid_pressed;
            }
        } else {
            return false;
        }
    }

} // namespace mge
