// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/pass.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    pass::pass(mge::render_context* context, uint32_t index) noexcept
        : m_context(context)
        , m_index(index)
    {}

    pass::~pass() {}

    void pass::touch()
    {
        m_active = true;
    }

} // namespace mge