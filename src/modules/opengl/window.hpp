// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#ifdef MGE_OS_WINDOWS
#    include "mge/win32/window.hpp"
#endif

namespace mge {
    namespace opengl {

        class window : public platform::window
        {
        public:
            window(const ::mge::extent &        extent,
                   const ::mge::window_options &options);
            ~window();
        };

    } // namespace opengl
} // namespace mge