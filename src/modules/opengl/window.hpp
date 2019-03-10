#pragma once
#include "mge/graphics/window.hpp"
#ifdef MGE_OS_WINDOWS
#include "win32/window.hpp"
#else
#error missing port
#endif
#include "opengl.hpp"

namespace opengl {
    class window : public platform::window
    {
    public:
        window(const mge::rectangle& rect,
               const mge::window_options& options);
        ~window();
    private:
        void create_render_target();
    };
}
