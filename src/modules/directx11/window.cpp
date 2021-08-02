// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_system.hpp"

namespace mge::dx11 {
    window::window(render_system &system, const mge::extent &extent,
                   const mge::window_options &options)
        : platform::window(extent, options), m_render_system(system)
    {}

    void window::create_render_context() {}

} // namespace mge::dx11