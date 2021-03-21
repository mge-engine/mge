// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/win32/render_system.hpp"
#include "mge/win32/monitor.hpp"

namespace mge {
    namespace win32 {
        render_system::render_system() : m_monitors(monitor::all_monitors()) {}
    } // namespace win32
} // namespace mge