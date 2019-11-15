// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/core/log.hpp"

MGE_DEFINE_LOG(GRAPHICS);

namespace mge {

    render_system::render_system()
    {}

    render_system::~render_system()
    {}

    std::string
    render_system::default_name()
    {
        configuration render_config("render");
        return render_config.value("system", std::string("opengl"));
    }

    render_system_ref
    render_system::create()
    {
        configuration render_system_config("render.system");
        auto system = component<render_system>::create(default_name());
        system->configure(render_system_config);
        return system;
    }

    render_system_ref
    render_system::create(const char *name)
    {
        return component<render_system>::create(name);
    }

    window_ref
    render_system::create_window()
    {
        rectangle r(0, 0, 800, 450);
        window_options options(window_options::RESIZABLE,
                               window_options::BORDER,
                               window_options::TITLE);
        return create_window(r, options);
    }

    MGE_REGISTER_COMPONENT(render_system);
}

