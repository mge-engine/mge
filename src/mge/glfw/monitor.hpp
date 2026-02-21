// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/monitor.hpp"
#include "mge/graphics/render_system.hpp"

#include <GLFW/glfw3.h>

#include <span>
#include <string>
#include <vector>

namespace mge {
    namespace glfw {

        class monitor : public mge::monitor
        {
        public:
            explicit monitor(GLFWmonitor* handle, bool primary);
            virtual ~monitor() = default;

            std::string_view      name() const override;
            bool                  primary() const override;
            extent                physical_size() const override;
            video_mode            current_mode() const override;
            video_mode_collection supported_modes() const override;

            static std::span<monitor_ref> all_monitors();

        private:
            GLFWmonitor* m_handle;
            bool         m_primary;
            std::string  m_name;
        };

    } // namespace glfw
} // namespace mge
