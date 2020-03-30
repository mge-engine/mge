// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/drawable.hpp"

namespace mge {

    drawable::drawable() : m_prepared(false) {}

    void drawable::prepare(render_context &context)
    {
        if (prepared()) {
            on_prepare(context);
            set_prepared(true);
        }
    }

    void drawable::on_draw(render_context &context)
    {
        if (!prepared()) {
            prepare(context);
        }
        on_draw(context);
    }

    void drawable::on_prepare(render_context &context) {}

    void drawable::set_prepared(bool prepared) noexcept
    {
        m_prepared = prepared;
    }

} // namespace mge
