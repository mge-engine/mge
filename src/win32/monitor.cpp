#include "win32/monitor.hpp"
#include "mge/core/to_utf8.hpp"
#include "mge/core/copy_struct.hpp"
#include "mge/core/system_error.hpp"

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
        return result;
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


    mge::monitor_ref
    monitor::primary_monitor()
    {
        mge::monitor_ref result;
        return result;
    }
}
