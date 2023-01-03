// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/input_state.hpp"

namespace mge {
    input_state::input_state() {}

    input_state::~input_state() {}

    keyboard_state& input_state::keyboard() { return m_keyboard; }

    const keyboard_state& input_state::keyboard() const { return m_keyboard; }

    const mouse_state& input_state::mouse() const { return m_mouse; }

    mouse_state& input_state::mouse() { return m_mouse; }
} // namespace mge
