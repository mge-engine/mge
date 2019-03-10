#include "window.hpp"
#include "window_render_target.hpp"

namespace opengl {

    window::window(const mge::rectangle& rect,
                   const mge::window_options& options)
        :platform::window(rect, options)
    {
        create_render_target();
    }

    window::~window()
    {}

    void
    window::create_render_target()
    {
        auto t = std::make_shared<window_render_target>(this);
        m_render_target = t;
    }
}
