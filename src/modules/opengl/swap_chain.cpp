// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "render_context.hpp"

namespace mge::opengl {
    swap_chain::swap_chain(render_context& context)
        : mge::swap_chain(context)
        , m_hdc(context.dc())
    {}

    void swap_chain::present()
    {
#ifdef MGE_OS_WINDOWS
        SwapBuffers(m_hdc);
#else
#    error Missing port
#endif
    }
} // namespace mge::opengl