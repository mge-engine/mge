// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/monitor.hpp"

#include <windows.h>

namespace mge {
    namespace win32 {
        class monitor : public mge::monitor
        {
        public:
            monitor(const DISPLAY_DEVICEW &adapter_device,
                    const DISPLAY_DEVICEW &display_device, bool primary);

            virtual ~monitor() = default;

            std::string_view      name() const override;
            bool                  primary() const override;
            extent                physical_size() const override;
            video_mode            current_mode() const override;
            video_mode_collection supported_modes() const override;

        private:
            DISPLAY_DEVICEW m_adapter_device;
            DISPLAY_DEVICEW m_display_device;
            bool            m_primary;
        };
    } // namespace win32
} // namespace mge
