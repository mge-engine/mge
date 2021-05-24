// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/stdexceptions.hpp"
namespace mge {
    MGE_REGISTER_COMPONENT(render_system);
    MGE_DEFINE_PARAMETER(std::string, graphics, render_system,
                         "Used render system implementation");

    render_system_ref render_system::create()
    {
        if (!MGE_PARAMETER(graphics, render_system).has_value()) {
            MGE_THROW(illegal_state)
                << "Parameter 'graphics.render_system' not set";
        }
        render_system_ref result;
        return result;
    }

    window_ref render_system::create_window()
    {
        return create_window(mge::extent(800, 600),
                             window_options::standard_options());
    }
} // namespace mge