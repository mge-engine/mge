// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "mge/graphics/swap_chain.hpp"
#include "mge/win32/com_ptr.hpp"

namespace mge::dx12 {

    class render_context;
    class render_system;

    class swap_chain : public mge::swap_chain
    {
    public:
        swap_chain(render_system& system, render_context& context);
        ~swap_chain();
        void present() override;

        IDXGISwapChain4* dxgi_swap_chain() const { return m_swap_chain.Get(); }

    private:
        mge::com_ptr<IDXGISwapChain4> m_swap_chain;
    };

} // namespace mge::dx12