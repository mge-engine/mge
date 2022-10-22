#include "window.hpp"

namespace mge::vulkan {
    window::window(const mge::extent&         extent,
                   const mge::window_options& options)
        : platform::window(extent, options)
    {

        create_render_context();
    }

    window::~window() {}

    void window::create_render_context() {}
} // namespace mge::vulkan