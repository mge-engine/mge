// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/frame_debugger.hpp"

#include "mge/core/parameter.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(GRAPHICS);
    MGE_REGISTER_COMPONENT(render_system);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(std::string,
                                      graphics,
                                      render_system,
                                      "Render system implementation",
                                      "opengl");
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(std::string,
                                      graphics,
                                      frame_debugger,
                                      "Frame debugger implementation",
                                      "");
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(bool,
                                      graphics,
                                      record_frames,
                                      "Record work using the frame debugger",
                                      false);

    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, graphics, vsync, "Honor vertical sync in screen update", false);

    render_system_ref render_system::create()
    {
        const char* env_value = std::getenv("MGE_RENDER_SYSTEM");
        std::string implementation_name;
        if (env_value == nullptr) {
            implementation_name = MGE_PARAMETER(graphics, render_system).get();
        } else {
            MGE_DEBUG_TRACE(GRAPHICS,
                            "Use render system from MGE_RENDER_SYSTEM: {}",
                            env_value);
            implementation_name = env_value;
        }
        auto result = component<render_system>::create(implementation_name);

        if (result) {
            const char* frame_debugger_env = std::getenv("MGE_FRAME_DEBUGGER");
            std::string frame_debugger_name =
                frame_debugger_env == nullptr
                    ? MGE_PARAMETER(graphics, frame_debugger).get()
                    : frame_debugger_env;
            if (!frame_debugger_name.empty()) {
                auto frame_debugger =
                    mge::frame_debugger::create(frame_debugger_name);
                if (frame_debugger) {
                    try {
                        frame_debugger->configure();
                        MGE_DEBUG_TRACE(GRAPHICS,
                                        "Using frame debugger: {} - {} {}",
                                        frame_debugger_name,
                                        frame_debugger->name(),
                                        frame_debugger->version());

                        result->m_frame_debugger = frame_debugger;
                    } catch (const std::exception& e) {
                        MGE_ERROR_TRACE(
                            GRAPHICS,
                            "Could not configure frame debugger '{}': {}",
                            frame_debugger_name,
                            e.what());
                    }
                } else {
                    MGE_WARNING_TRACE(
                        GRAPHICS,
                        "Frame debugger '{}' could not be created",
                        frame_debugger_name);
                }
            } else {
                MGE_DEBUG_TRACE(GRAPHICS, "No frame debugger specified");
            }
        }

        return result;
    }

    render_system_ref render_system::create(std::string_view implementation)
    {
        std::string implementation_name(implementation);
        return component<render_system>::create(implementation_name);
    }

    window_ref render_system::create_window()
    {
        return create_window(mge::extent(800, 600),
                             window_options::standard_options());
    }

    const mge::render_system::capabilities&
    render_system::system_capabilities() const
    {
        return *m_capabilities;
    }

    render_system::capabilities::capabilities() {}

    render_system::capabilities::~capabilities() {}

    std::span<const shader_language>
    render_system::capabilities::shader_languages() const
    {
        return std::span<const shader_language>();
    }

    std::span<const shader_format>
    render_system::capabilities::shader_formats() const
    {
        return std::span<const shader_format>();
    }

    frame_debugger_ref render_system::frame_debugger() const
    {
        return m_frame_debugger;
    }

    render_system::~render_system()
    {
        if (m_frame_debugger && m_frame_debugger->capturing()) {
            MGE_DEBUG_TRACE(GRAPHICS,
                            "Stopping recording in frame debugger: {}",
                            m_frame_debugger->name());
            m_frame_debugger->end_capture();
        }
    }

} // namespace mge