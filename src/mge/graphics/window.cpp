// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/window.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {
    window::window(const mge::extent& extent_, const window_options&)
        : m_extent(extent_)
        , m_closing_listener([] { return true; })
        , m_visible(false)
    {}

    window::~window() { m_render_context.reset(); }

    void window::show()
    {
        if (!m_visible) {
            on_show();
            m_visible = true;
        }
    }

    void window::hide()
    {
        if (m_visible) {
            on_hide();
            m_visible = false;
        }
    }

    bool window::visible() const { return m_visible; }

    point window::position() const { return m_position; }

    mge::extent window::extent() const { return m_extent; }

    mge::render_context& window::render_context() { return *m_render_context; }

    const mge::render_context& window::render_context() const { return *m_render_context; }

    void window::set_close_listener(const window::close_listener& listener) { m_close_listener = listener; }

    void window::clear_close_listener() { m_close_listener = void_function(); }

    void window::set_closing_listener(const window::closing_listener& listener)
    {
        if (!m_closing_listener) {
            clear_closing_listener();
        }
        m_closing_listener = listener;
    }

    void window::clear_closing_listener()
    {
        m_close_listener = [] { return true; };
    }

    bool window::on_close()
    {
        if (!m_closing_listener()) {
            return false;
        }
        if (m_close_listener) {
            m_close_listener();
        }
        return true;
    }
} // namespace mge
