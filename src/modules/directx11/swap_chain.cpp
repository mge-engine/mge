// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {
    swap_chain::swap_chain(render_context& context, IDXGISwapChain* swap_chain)
        : mge::swap_chain(context)
    {
        m_swap_chain.reset(swap_chain);

        D3D11_TEXTURE2D_DESC depth_buffer_desc = {};
        depth_buffer_desc.Width = context.window().extent().width;
        depth_buffer_desc.Height = context.window().extent().height;
        depth_buffer_desc.MipLevels = 1;
        depth_buffer_desc.ArraySize = 1;
        depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_buffer_desc.SampleDesc.Count = 1;
        depth_buffer_desc.SampleDesc.Quality = 0;
        depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depth_buffer_desc.CPUAccessFlags = 0;
        depth_buffer_desc.MiscFlags = 0;

        MGE_DEBUG_TRACE_STREAM(DX11) << "Creating depth/stencil buffer texture";
        ID3D11Texture2D* tmp_depth_stencil_buffer;

        auto rc = context.device()->CreateTexture2D(&depth_buffer_desc,
                                                    nullptr,
                                                    &tmp_depth_stencil_buffer);
        CHECK_HRESULT(rc,
                      ID3D11Device,
                      CreateTexture2D(D3D11_BIND_DEPTH_STENCIL));
        m_depth_stencil_buffer.reset(tmp_depth_stencil_buffer);

        MGE_DEBUG_TRACE_STREAM(DX11) << "Creating depth/stencil state";
        D3D11_DEPTH_STENCIL_DESC depth_stencil_state_desc = {};
        depth_stencil_state_desc.DepthEnable = true;
        depth_stencil_state_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depth_stencil_state_desc.DepthFunc = D3D11_COMPARISON_LESS;

        depth_stencil_state_desc.StencilEnable = true;
        depth_stencil_state_desc.StencilReadMask = 0xFF;
        depth_stencil_state_desc.StencilWriteMask = 0xFF;

        // Stencil operations if pixel is front-facing.
        depth_stencil_state_desc.FrontFace.StencilFailOp =
            D3D11_STENCIL_OP_KEEP;
        depth_stencil_state_desc.FrontFace.StencilDepthFailOp =
            D3D11_STENCIL_OP_INCR;
        depth_stencil_state_desc.FrontFace.StencilPassOp =
            D3D11_STENCIL_OP_KEEP;
        depth_stencil_state_desc.FrontFace.StencilFunc =
            D3D11_COMPARISON_ALWAYS;

        // Stencil operations if pixel is back-facing.
        depth_stencil_state_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depth_stencil_state_desc.BackFace.StencilDepthFailOp =
            D3D11_STENCIL_OP_DECR;
        depth_stencil_state_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depth_stencil_state_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        ID3D11DepthStencilState* tmp_depth_stencil_state = nullptr;
        rc =
            context.device()->CreateDepthStencilState(&depth_stencil_state_desc,
                                                      &tmp_depth_stencil_state);
        CHECK_HRESULT(rc, ID3D11Device, CreateDeptHStencilState);
        m_depth_stencil_state.reset(tmp_depth_stencil_state);
    }

    com_unique_ptr<ID3D11Texture2D> swap_chain::back_buffer() const
    {
        ID3D11Texture2D* tmp_back_buffer = nullptr;
        m_swap_chain->GetBuffer(0,
                                __uuidof(ID3D11Texture2D),
                                reinterpret_cast<LPVOID*>(&tmp_back_buffer));
        if (!tmp_back_buffer) {
            MGE_THROW(dx11::error)
                << "Failed to get back buffer from swap chain";
        }
        com_unique_ptr<ID3D11Texture2D> back_buffer;
        back_buffer.reset(tmp_back_buffer);
        return back_buffer;
    }

    swap_chain::~swap_chain() {}

    void swap_chain::present()
    {
        auto hr = m_swap_chain->Present(0, 0);
        CHECK_HRESULT(hr, IDXGISwapChain, Present);
    }
} // namespace mge::dx11