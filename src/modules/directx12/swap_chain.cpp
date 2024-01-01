// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "error.hpp"
#include "render_context.hpp"
#include "render_system.hpp"

namespace mge::dx12 {

    swap_chain::swap_chain(render_system& system, render_context& context)
        : mge::swap_chain(context)
        , m_frame_index(0)
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {
            .Width = context.window().extent().width,
            .Height = context.window().extent().height,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .SampleDesc = {.Count = 1, .Quality = 0},
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = render_context::buffer_count,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
            .Flags = system.tearing_support()
                         ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING
                         : 0u,
        };
        mge::com_ptr<IDXGISwapChain1> swapchain1;
        auto rc = context.factory()->CreateSwapChainForHwnd(
            context.command_queue().Get(),
            context.window().hwnd(),
            &desc,
            nullptr,
            nullptr,
            &swapchain1);
        CHECK_HRESULT(rc, IDXGIFactory4, CreateSwapChainForHwnd);
        // TODO: #123 Support Alt+Enter handling for window size change back and
        // forth
        rc = context.factory()->MakeWindowAssociation(context.window().hwnd(),
                                                      DXGI_MWA_NO_ALT_ENTER);

        CHECK_HRESULT(rc, IDXGIFactory4, MakeWindowAssociation);
        rc = swapchain1.As(&m_swap_chain);
        CHECK_HRESULT(rc, IDXGISwapChain1, As);
        m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();
    }

    void swap_chain::present() { m_swap_chain->Present(0, 0); }

} // namespace mge::dx12