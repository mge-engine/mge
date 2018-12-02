// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "opengl.hpp"
#include "mge/graphics/render_system.hpp"
#ifdef MGE_OS_WINDOWS
#  include "win32/monitor.hpp"
#endif

namespace opengl {
    class render_system : public mge::render_system
    {
    public:
        render_system() = default;
        virtual ~render_system() = default;

        monitor_collection_t monitors() const override
        {
            return ::platform::monitor::all_monitors();
        }

        mge::monitor_ref primary_monitor() const override
        {
            return platform::monitor::primary_monitor();
        }

        mge::window_ref create_window(const mge::rectangle& rect,
                                      const mge::window_options& options) override
        {
            mge::window_ref result;
            return result;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                opengl);
}
