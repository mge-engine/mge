// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/graphics/swap_chain.hpp"
#include "opengl.hpp"
namespace mge::opengl {

    class render_context;

    class swap_chain : public mge::swap_chain
    {
    public:
        swap_chain(render_context& context);
        ~swap_chain() = default;
        void     present() override;
        uint32_t current_back_buffer_index() const override { return 0; }

#ifdef MGE_OS_WINDOWS
    private:
        HDC m_hdc;
#else
#    error Missing port
#endif
    };

} // namespace mge::opengl