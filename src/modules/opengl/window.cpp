#include "window.hpp"
#include "render_context.hpp"

namespace opengl {

    window::window(const mge::rectangle& rect,
                   const mge::window_options& options)
        :platform::window(rect, options)
    {
        create_render_context();
    }

    window::~window()
    {}

    void
    window::create_render_context()
    {
        auto t = std::make_shared<opengl::render_context>(this);
        m_render_context = t;
    }

    void
    window::assign_thread()
    {
        m_render_context->assign_thread();
    }
}
