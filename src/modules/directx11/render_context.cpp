#include "render_context.hpp"
#include "render_system.hpp"
#include "window.hpp"

namespace mge::dx11 {
    render_context::render_context(mge::dx11::render_system &render_system_,
                                   mge::dx11::window &       window_)
        : m_render_system(render_system_), m_window(window_)
    {}

    render_context::~render_context() {}
} // namespace mge::dx11