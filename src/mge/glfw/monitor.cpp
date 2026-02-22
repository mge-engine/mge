// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/glfw/monitor.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(GLFW);

    namespace glfw {

        namespace {
            struct monitors_cache
            {
                monitors_cache()
                {
                    if (!glfwInit()) {
                        MGE_THROW(mge::runtime_exception) << "glfwInit failed";
                    }
                    refresh();
                }

                ~monitors_cache()
                {
                    m_refs.clear();
                    glfwTerminate();
                }

                void refresh()
                {
                    m_refs.clear();
                    int           count = 0;
                    GLFWmonitor** handles = glfwGetMonitors(&count);
                    GLFWmonitor*  primary = glfwGetPrimaryMonitor();
                    for (int i = 0; i < count; ++i) {
                        m_refs.push_back(
                            std::make_shared<monitor>(handles[i],
                                                      handles[i] == primary));
                    }
                }

                std::vector<monitor_ref> m_refs;
            };
        } // namespace

        monitor::monitor(GLFWmonitor* handle, bool primary)
            : m_handle(handle)
            , m_primary(primary)
            , m_name(glfwGetMonitorName(handle))
        {}

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
            int width_mm = 0;
            int height_mm = 0;
            glfwGetMonitorPhysicalSize(m_handle, &width_mm, &height_mm);
            return extent(static_cast<uint32_t>(width_mm),
                          static_cast<uint32_t>(height_mm));
        }

        video_mode monitor::current_mode() const
        {
            const GLFWvidmode* mode = glfwGetVideoMode(m_handle);
            return video_mode(static_cast<uint32_t>(mode->width),
                              static_cast<uint32_t>(mode->height),
                              static_cast<uint32_t>(mode->refreshRate));
        }

        monitor::video_mode_collection monitor::supported_modes() const
        {
            int                   count = 0;
            const GLFWvidmode*    modes = glfwGetVideoModes(m_handle, &count);
            video_mode_collection result;
            result.reserve(static_cast<size_t>(count));
            for (int i = 0; i < count; ++i) {
                result.emplace_back(
                    static_cast<uint32_t>(modes[i].width),
                    static_cast<uint32_t>(modes[i].height),
                    static_cast<uint32_t>(modes[i].refreshRate));
            }
            return result;
        }

        std::span<monitor_ref> monitor::all_monitors()
        {
            static monitors_cache cache;
            return std::span<monitor_ref>(cache.m_refs);
        }

    } // namespace glfw
} // namespace mge
