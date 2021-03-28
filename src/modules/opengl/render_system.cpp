// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif
namespace mge {
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

            monitor_collection monitors()
            {
#ifdef MGE_OS_WINDOWS
                return mge::win32::monitor::all_monitors();
#endif
            }
        };

        MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, opengl);
    } // namespace opengl
} // namespace mge