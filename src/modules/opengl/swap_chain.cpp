// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "mge/core/stdexceptions.hpp"
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

    image_ref swap_chain::screenshot()
    {
        MGE_THROW(not_implemented) << "Screenshot not yet implemented";
    }
} // namespace mge::opengl