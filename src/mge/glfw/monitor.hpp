// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/monitor.hpp"

#include <span>

struct GLFWmonitor;

namespace mge::glfw {

    class monitor : public mge::monitor
    {
    public:
        explicit monitor(GLFWmonitor* glfw_monitor);
        virtual ~monitor() = default;

        std::string_view      name() const override;
        bool                  primary() const override;
        extent                physical_size() const override;
        video_mode            current_mode() const override;
        video_mode_collection supported_modes() const override;

        static std::span<monitor_ref> all_monitors();

    private:
        friend class all_monitors_collection;

        void compute_video_modes();

        GLFWmonitor*          m_glfw_monitor;
        std::string           m_name;
        video_mode_collection m_video_modes;
        video_mode            m_current_mode;
        extent                m_physical_size;
        bool                  m_primary;
    };

} // namespace mge::glfw
