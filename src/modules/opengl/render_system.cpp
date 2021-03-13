// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"

namespace opengl {
    class render_system : public mge::render_system
    {
    public:
        render_system()  = default;
        ~render_system() = default;

        mge::window_ref create_window(const mge::extent &        extent,
                                      const mge::window_options &options)
        {
            mge::window_ref ref;
            return ref;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, opengl);
} // namespace opengl