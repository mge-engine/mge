// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/window.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {
    window::window(const mge::extent &, const window_options &)
        : m_visible(false)
    {}

    window::~window() {}

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

    mge::render_context &window::render_context() { return *m_render_context; }

} // namespace mge
