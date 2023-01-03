// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/monitor.hpp"
#include "mge/graphics/render_system.hpp"

#include <windows.h>

namespace mge {
    namespace win32 {
        class monitor : public mge::monitor
        {
        public:
            using monitor_collection = mge::render_system::monitor_collection;

            monitor(const DISPLAY_DEVICEW& adapter_device,
                    const DISPLAY_DEVICEW& display_device,
                    bool                   primary);

            virtual ~monitor() = default;

            std::string_view      name() const override;
            bool                  primary() const override;
            extent                physical_size() const override;
            video_mode            current_mode() const override;
            video_mode_collection supported_modes() const override;

            static monitor_collection all_monitors();

        private:
            void compute_physical_size();
            void compute_video_modes();

            DISPLAY_DEVICEW       m_adapter_device;
            DISPLAY_DEVICEW       m_display_device;
            video_mode_collection m_video_modes;
            std::string           m_name;
            video_mode            m_current_mode;
            extent                m_physical_size;
            bool                  m_primary;
        };
    } // namespace win32
} // namespace mge
