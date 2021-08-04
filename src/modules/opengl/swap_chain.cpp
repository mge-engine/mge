// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "render_context.hpp"

namespace mge::opengl {
    swap_chain::swap_chain(render_context &context) : mge::swap_chain(context)
    {}

    void swap_chain::present() {}
} // namespace mge::opengl