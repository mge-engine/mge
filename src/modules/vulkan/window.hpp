#pragma once
#include "mge/config.hpp"
#ifdef MGE_OS_WINDOWS
#    include "mge/win32/window.hpp"
#else
#    error Missing port
#endif

namespace mge::vulkan {

    class render_system;

    class window : public platform::window
    {
    public:
        window(render_system&               render_system_,
               const ::mge::extent&         extent,
               const ::mge::window_options& options);
        ~window();

    private:
        void create_render_context();

        render_system& m_render_system;
    };
} // namespace mge::vulkan