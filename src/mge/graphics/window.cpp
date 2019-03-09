#include "mge/graphics/window.hpp"
#include "mge/core/clear_function.hpp"
namespace mge {
    window::window(const rectangle &rect,
                   const window_options &options)
        :m_rect(rect)
        ,m_options(options)
        ,m_visible(false)
    {}

    window::~window()
    {
        m_render_target.reset();
    }

    const render_target&
    window::render_target() const
    {
        return *m_render_target;
    }

    render_target&
    window::render_target()
    {
        return *m_render_target;
    }

    void
    window::show()
    {
        if(!m_visible) {
            on_show();
            m_visible = true;
        }
    }

    void
    window::hide()
    {
        if(m_visible) {
            on_hide();
            m_visible = false;
        }
    }

    bool
    window::visible() const
    {
        return m_visible;
    }

    void
    window::set_redraw_listener(const window::redraw_listener& listener)
    {
        m_redraw_listener = listener;
    }

    void
    window::clear_redraw_listener()
    {
        clear_function(m_redraw_listener);
    }

    void
    window::set_close_listener(const window::close_listener& listener)
    {
        m_close_listener = listener;
    }

    void
    window::clear_close_listener()
    {
        clear_function(m_close_listener);
    }

    void
    window::set_closing_listener(const window::closing_listener& listener)
    {
        m_closing_listener = listener;
    }

    void
    window::clear_closing_listener()
    {
        clear_function(m_closing_listener);
    }

    void
    window::refresh(float interpolation)
    {
        if(m_redraw_listener) {
            m_redraw_listener(render_target(), interpolation);
        }
    }

    void
    window::close()
    {
        request_close();
    }

    void
    window::on_close()
    {

    }

}
