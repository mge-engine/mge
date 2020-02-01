// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "opengl.hpp"
#include "window.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/core/log.hpp"

#ifdef MGE_OS_WINDOWS
#  include "win32/monitor.hpp"
#endif

MGE_USE_LOG(OPENGL);

namespace opengl {
    class render_system : public mge::render_system
    {
    public:
        render_system()
        {
            MGE_DEBUG_LOG(OPENGL) << "Creating OpenGL render system";
        }
        virtual ~render_system() = default;

        monitor_collection_type monitors() const override
        {
            return ::mge::platform::monitor::all_monitors();
        }

        mge::monitor_ref primary_monitor() const override
        {
            return ::mge::platform::monitor::primary_monitor();
        }

        mge::window_ref create_window(const mge::rectangle& rect,
                                      const mge::window_options& options) override
        {
            mge::window_ref result = std::make_shared<window>(rect, options);
            return result;
        }


    };

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                opengl);
}
