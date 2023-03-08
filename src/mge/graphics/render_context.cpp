// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "mge/core/parameter.hpp"

namespace mge {
    render_context::render_context() {}

    const swap_chain_ref& render_context::swap_chain() const
    {
        return m_swap_chain;
    }

} // namespace mge