// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
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
        ~swap_chain() = default;
        void present() override;

        IDXGISwapChain4* dxgi_swap_chain() const { return m_swap_chain.Get(); }

        uint32_t current_back_buffer_index() const override;

    private:
        mge::com_ptr<IDXGISwapChain4> m_swap_chain;
    };

    inline swap_chain& dx12_swap_chain(mge::swap_chain& sc)
    {
        return static_cast<swap_chain&>(sc);
    }

    inline const swap_chain& dx12_swap_chain(const mge::swap_chain& sc)
    {
        return static_cast<const swap_chain&>(sc);
    }

} // namespace mge::dx12