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
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {};
        desc.Width = 0;
        desc.Height = 0;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo = FALSE;
        desc.SampleDesc = {1, 0};
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount =
            render_context::buffer_count; // TODO: multi-buffering for DirectX12
        desc.Scaling = DXGI_SCALING_STRETCH;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        desc.Flags =
            system.tearing_support() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
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
    }

    swap_chain::~swap_chain() {}

    void swap_chain::present() { m_swap_chain->Present(0, 0); }

} // namespace mge::dx12