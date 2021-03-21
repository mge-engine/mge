// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/win32/monitor.hpp"
#include "mge/core/copy_struct.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/to_utf8.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/zero_memory.hpp"

MGE_USE_IMPORTED_TRACE(WIN32);

namespace mge {
    namespace win32 {

        monitor::monitor(const DISPLAY_DEVICEW &adapter_device,
                         const DISPLAY_DEVICEW &display_device, bool primary)
            : m_primary(primary)
        {
            copy_struct(m_adapter_device, adapter_device);
            copy_struct(m_display_device, display_device);
            compute_physical_size();
            compute_video_modes();
        }

        void monitor::compute_physical_size()
        {
            HDC dc;
            dc = CreateDCW(L"DISPLAY", m_adapter_device.DeviceName, NULL, NULL);
            if (!dc) {
                MGE_THROW(mge::system_error)
                    << "CreateDC call for device '"
                    << mge::to_utf8(m_adapter_device.DeviceName) << "' failed";
            }

            m_physical_size.width  = (uint32_t)GetDeviceCaps(dc, HORZSIZE);
            m_physical_size.height = (uint32_t)GetDeviceCaps(dc, VERTSIZE);
            DeleteDC(dc);
        }

        void monitor::compute_video_modes()
        {
            for (DWORD i = 0;; ++i) {
                DEVMODEW dm;
                mge::zero_memory(dm);
                dm.dmSize = sizeof(DEVMODEW);
                if (!EnumDisplaySettingsW(m_adapter_device.DeviceName, i,
                                          &dm)) {
                    break;
                }

                m_video_modes.emplace_back(dm.dmPelsWidth, dm.dmPelsHeight,
                                           dm.dmDisplayFrequency);
            }
        }

        monitor::monitor_collection monitor::all_monitors()
        {
            monitor_collection result;
            return result;
        }

    } // namespace win32

} // namespace mge