// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/win32/monitor.hpp"
#include "mge/core/copy_struct.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/to_utf8.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/zero_memory.hpp"

#ifdef WIN32
#    undef WIN32
#endif

namespace mge {

    MGE_USE_TRACE(WIN32);

    namespace win32 {

        monitor::monitor(const DISPLAY_DEVICEW& adapter_device,
                         const DISPLAY_DEVICEW& display_device,
                         bool                   primary)
            : m_primary(primary)
        {
            copy_struct(m_adapter_device, adapter_device);
            copy_struct(m_display_device, display_device);
            compute_physical_size();
            compute_video_modes();
            m_name = mge::to_utf8(m_display_device.DeviceName);
        }

        std::string_view monitor::name() const
        {
            return std::string_view(m_name.begin(), m_name.end());
        }

        bool monitor::primary() const
        {
            return m_primary;
        }

        mge::extent monitor::physical_size() const
        {
            return m_physical_size;
        }

        video_mode monitor::current_mode() const
        {
            return m_current_mode;
        }

        monitor::video_mode_collection monitor::supported_modes() const
        {
            return m_video_modes;
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

            m_physical_size.width = (uint32_t)GetDeviceCaps(dc, HORZSIZE);
            m_physical_size.height = (uint32_t)GetDeviceCaps(dc, VERTSIZE);
            DeleteDC(dc);
        }

        void monitor::compute_video_modes()
        {
            for (DWORD i = 0;; ++i) {
                DEVMODEW dm;
                mge::zero_memory(dm);
                dm.dmSize = sizeof(DEVMODEW);
                if (!EnumDisplaySettingsW(m_adapter_device.DeviceName,
                                          i,
                                          &dm)) {
                    break;
                }

                m_video_modes.emplace_back(dm.dmPelsWidth,
                                           dm.dmPelsHeight,
                                           dm.dmDisplayFrequency);
            }

            DEVMODEW dm;
            mge::zero_memory(dm);
            dm.dmSize = sizeof(DEVMODEW);
            if (EnumDisplaySettingsW(m_adapter_device.DeviceName,
                                     ENUM_CURRENT_SETTINGS,
                                     &dm)) {
                m_current_mode = video_mode(dm.dmPelsWidth,
                                            dm.dmPelsHeight,
                                            dm.dmDisplayFrequency);
            } else {
                // handle error;
            }
        }

        class all_monitors_collection : public std::vector<monitor_ref>
        {
        public:
            all_monitors_collection()
            {
                DISPLAY_DEVICEW adapter_device;
                for (DWORD adapter_index = 0;; ++adapter_index) {
                    mge::zero_memory(adapter_device);
                    adapter_device.cb = sizeof(adapter_device);
                    if (!EnumDisplayDevicesW(nullptr,
                                             adapter_index,
                                             &adapter_device,
                                             0)) {
                        break;
                    }
                    MGE_DEBUG_TRACE_STREAM(WIN32)
                        << "Found adapter: "
                        << mge::to_utf8(adapter_device.DeviceName) << " - "
                        << mge::to_utf8(adapter_device.DeviceString);
                    DISPLAY_DEVICEW display_device;
                    for (DWORD display_index = 0;; ++display_index) {
                        mge::zero_memory(display_device);
                        display_device.cb = sizeof(display_device);
                        if (!EnumDisplayDevicesW(adapter_device.DeviceName,
                                                 display_index,
                                                 &display_device,
                                                 0)) {
                            break;
                        }
                        emplace_back(std::make_shared<monitor>(adapter_device,
                                                               display_device,
                                                               empty()));
                    }
                }
            }
        };

        static mge::singleton<all_monitors_collection> s_all_monitors_singleton;

        std::span<monitor_ref> monitor::all_monitors()
        {
            return std::span<monitor_ref>(s_all_monitors_singleton->begin(),
                                          s_all_monitors_singleton->end());
        }

    } // namespace win32

} // namespace mge