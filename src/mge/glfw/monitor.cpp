// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/glfw/monitor.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"
#include "mge/glfw/glfw_instance.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace mge {
    MGE_USE_TRACE(GLFW);
}

namespace mge::glfw {

    monitor::monitor(GLFWmonitor* glfw_monitor)
        : m_glfw_monitor(glfw_monitor)
        , m_name(glfwGetMonitorName(glfw_monitor))
        , m_primary(false)
    {
        // Get physical size in mm
        int width_mm, height_mm;
        glfwGetMonitorPhysicalSize(m_glfw_monitor, &width_mm, &height_mm);
        m_physical_size = extent(static_cast<uint32_t>(width_mm),
                                 static_cast<uint32_t>(height_mm));

        // Get current video mode
        const GLFWvidmode* mode = glfwGetVideoMode(m_glfw_monitor);
        if (mode) {
            m_current_mode =
                video_mode(static_cast<uint32_t>(mode->width),
                           static_cast<uint32_t>(mode->height),
                           static_cast<uint32_t>(mode->refreshRate));
        }

        compute_video_modes();
    }

    std::string_view monitor::name() const
    {
        return m_name;
    }

    bool monitor::primary() const
    {
        return m_primary;
    }

    extent monitor::physical_size() const
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

    void monitor::compute_video_modes()
    {
        int                count;
        const GLFWvidmode* modes = glfwGetVideoModes(m_glfw_monitor, &count);

        m_video_modes.clear();
        m_video_modes.reserve(count);

        for (int i = 0; i < count; ++i) {
            m_video_modes.emplace_back(
                static_cast<uint32_t>(modes[i].width),
                static_cast<uint32_t>(modes[i].height),
                static_cast<uint32_t>(modes[i].refreshRate));
        }
    }

    class all_monitors_collection : public std::vector<monitor_ref>
    {
    public:
        all_monitors_collection()
        {
            // Ensure GLFW is initialized
            glfw_instance::instance();

            int           count;
            GLFWmonitor** monitors = glfwGetMonitors(&count);
            GLFWmonitor*  primary = glfwGetPrimaryMonitor();

            if (monitors && count > 0) {
                reserve(count);
                for (int i = 0; i < count; ++i) {
                    auto mon = std::make_shared<monitor>(monitors[i]);
                    if (monitors[i] == primary) {
                        mon->m_primary = true;
                    }
                    emplace_back(std::move(mon));
                    MGE_DEBUG_TRACE(GLFW, "Found monitor: {}", back()->name());
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

} // namespace mge::glfw
