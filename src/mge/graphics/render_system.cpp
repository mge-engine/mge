// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"
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
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, graphics, vsync, "Honor vertical sync in screen update", false);

    render_system_ref render_system::create()
    {
        const char* env_value = std::getenv("MGE_RENDER_SYSTEM");
        std::string implementation_name;
        if (env_value == nullptr) {
            implementation_name = MGE_PARAMETER(graphics, render_system).get();
        } else {
            MGE_DEBUG_TRACE(GRAPHICS)
                << "Use render system from MGE_RENDER_SYSTEM: " << env_value;
            implementation_name = env_value;
        }
        return component<render_system>::create(implementation_name);
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

} // namespace mge