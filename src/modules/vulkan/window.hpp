#pragma once
#include "mge/config.hpp"
#fdef MGE_OS_WINDOWS
#include "mge/win32/window.hpp"
#endif

namespace mge {
    namespace vulkan {

        class window : public platform::window
        {
        public:
            window(const ::mge::extent &        extent,
                   const ::mge::window_options &options);
            ~window();
        };
    } // namespace vulkan
} // namespace mge