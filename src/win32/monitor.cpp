#include "win32/monitor.hpp"
#include "mge/core/to_utf8.hpp"
#include "mge/core/copy_struct.hpp"
#include "mge/core/zero_memory.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(WIN32);

namespace win32 {


    monitor::monitor(const DISPLAY_DEVICEW &adapter_device,
                     const DISPLAY_DEVICEW &device,
                     bool primary)
        :mge::monitor(mge::to_utf8(device.DeviceString),
                      primary)
        ,m_physical_width(0)
        ,m_physical_height(0)
    {
        mge::copy_struct(m_adapter_device, adapter_device);
        mge::copy_struct(m_device, device);

        HDC dc;
        dc = CreateDCW(L"DISPLAY", adapter_device.DeviceName, NULL, NULL);
        if (!dc) {
            MGE_THROW(mge::system_error(),
                      "CreateDC call for device ",
                      mge::to_utf8(adapter_device.DeviceName),
                      " failed");
        }
        m_physical_width = (unsigned int) GetDeviceCaps(dc, HORZSIZE);
        m_physical_height = (unsigned int) GetDeviceCaps(dc, VERTSIZE);
        DeleteDC(dc);
    }

    unsigned int
    monitor::physical_height() const
    {
        return m_physical_height;
    }

    unsigned int
    monitor::physical_width() const
    {
        return m_physical_width;
    }

    monitor::monitor_collection_t
    monitor::all_monitors()
    {
        monitor_collection_t result;

        DISPLAY_DEVICEW adapter_device;
        for(DWORD adapter_index=0; ;++adapter_index) {
            mge::zero_memory(adapter_device);
            adapter_device.cb = sizeof(adapter_device);
            if(!EnumDisplayDevicesW(nullptr, adapter_index, &adapter_device, 0)) {
                break;
            }
            MGE_DEBUG_LOG(WIN32) << "Found adapter: " << mge::to_utf8(adapter_device.DeviceName)
                                 << " - " << mge::to_utf8(adapter_device.DeviceString);
            DISPLAY_DEVICEW display_device;
            for (DWORD display_index=0; ;++display_index) {
                mge::zero_memory(display_device);
                display_device.cb = sizeof(display_device);
                if(!EnumDisplayDevicesW(adapter_device.DeviceName, display_index, &display_device, 0)) {
                    break;
                }
                result.emplace_back(std::make_shared<monitor>(adapter_device,
                                                              display_device,
                                                              result.empty()));
            }
        }
        return result;
    }

    mge::monitor::video_mode_collection_t
    monitor::video_modes() const
    {
        mge::monitor::video_mode_collection_t result;
        for(DWORD i=0;;++i) {
            DEVMODEW dm;
            mge::zero_memory(dm);
            dm.dmSize = sizeof(DEVMODEW);
            if(!EnumDisplaySettingsW(m_adapter_device.DeviceName,
                                     i,
                                     &dm)) {
                break;
            }
            mge::video_mode mode;
            mode.width = dm.dmPelsWidth;
            mode.height = dm.dmPelsHeight;
            mode.refresh_rate = dm.dmDisplayFrequency;
            result.emplace_back(mode);
        }
        return result;
    }

    void
    monitor::set_video_mode(const mge::video_mode& mode)
    {
        return;
    }

    mge::gamma_ramp
    monitor::gamma_ramp() const
    {
        WORD values[768];
        HDC dc = CreateDCW(L"DISPLAY", m_adapter_device.DeviceName,
                           nullptr,
                           nullptr);
        if (!dc) {
            MGE_THROW(mge::system_error(),
                      "CreateDC call for device ",
                      mge::to_utf8(m_adapter_device.DeviceName),
                      " failed");
        }
        BOOL rc = GetDeviceGammaRamp(dc, values);
        if (!rc) {
            MGE_THROW(mge::system_error(),
                      "GetDeviceGammaRamp call for device ",
                      mge::to_utf8(m_adapter_device.DeviceName),
                      " failed");
        }
        DeleteDC(dc);

        mge::gamma_ramp result;
        memcpy(result.red_ramp(), values, 256*sizeof(unsigned short));
        memcpy(result.green_ramp(),values + 256, 256*sizeof(unsigned short));
        memcpy(result.blue_ramp(), values + 512, 256*sizeof(unsigned short));

        return result;
    }

    mge::video_mode
    monitor::current_video_mode() const
    {
        DEVMODEW dm;
        mge::zero_memory(dm);
        dm.dmSize = sizeof(DEVMODEW);
        if(!EnumDisplaySettingsW(m_adapter_device.DeviceName,
                                 ENUM_CURRENT_SETTINGS,
                                 &dm))  {
            MGE_THROW(mge::system_error(),
                      "EnumDisplaySettingsW call for device ",
                      mge::to_utf8(m_adapter_device.DeviceName),
                      " failed");
        }
        mge::video_mode mode;
        mode.width = dm.dmPelsWidth;
        mode.height = dm.dmPelsHeight;
        mode.refresh_rate = dm.dmDisplayFrequency;
        return mode;
    }


    mge::monitor_ref
    monitor::primary_monitor()
    {
        auto monitors = all_monitors();
        for(const auto& m: monitors) {
            if(m->primary()) {
                return m;
            }
        }
        return mge::monitor_ref();
    }
}
