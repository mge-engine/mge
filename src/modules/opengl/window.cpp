#include "window.hpp"
#include "window_render_context.hpp"

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
        auto t = std::make_shared<window_render_context>(this);
        m_render_context = t;
    }
}
