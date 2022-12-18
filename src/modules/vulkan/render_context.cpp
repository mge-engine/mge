#include "render_context.hpp"
#include "render_system.hpp"
#include "window.hpp"

namespace mge::vulkan {
    render_context::render_context(render_system& system_, window& window_)
        : m_render_system(system_)
        , m_window(window_)
    {}

    render_context::~render_context() {}
} // namespace mge::vulkan