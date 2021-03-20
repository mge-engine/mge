// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/win32/monitor.hpp"
#include "mge/core/copy_struct.hpp"

namespace mge {
    namespace win32 {

        monitor::monitor(const DISPLAY_DEVICEW &adapter_device,
                         const DISPLAY_DEVICEW &display_device, bool primary)
            : m_primary(primary)
        {
            copy_struct(m_adapter_device, adapter_device);
            copy_struct(m_display_device, display_device);
        }

    } // namespace win32
} // namespace mge