#pragma once
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/monitor.hpp"
#include <windows.h>

namespace win32 {
    class monitor : public mge::monitor
    {
    public:
        monitor(const DISPLAY_DEVICEW& adapter_device,
                const DISPLAY_DEVICEW& device,
                bool primary);
        ~monitor() = default;
        unsigned int physical_width() const override;
        unsigned int physical_height() const override;
        mge::gamma_ramp gamma_ramp() const override;
        //void video_modes(std::vector<moge::video_mode>& modes);
         //       void set_video_mode(const moge::video_mode& mode);

        typedef mge::render_system::monitor_collection_t monitor_collection_t;
        static monitor_collection_t all_monitors();
        static mge::monitor_ref primary_monitor();

    private:
        DISPLAY_DEVICEW m_adapter_device;
        DISPLAY_DEVICEW m_device;
        unsigned int m_physical_width;
        unsigned int m_physical_height;
    };
}
