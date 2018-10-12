// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"
#include "mge/core/configuration.hpp"

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
        return component<render_system>::create(default_name());
    }

    MGE_REGISTER_COMPONENT(render_system);
}
