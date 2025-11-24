// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/swap_chain.hpp"
#include "mge/win32/com_unique_ptr.hpp"
#include "render_context.hpp"

namespace mge::dx11 {

    class swap_chain : public mge::swap_chain
    {
    public:
        swap_chain(render_context& context, IDXGISwapChain* swap_chain);
        ~swap_chain();

        void present() override;

        com_unique_ptr<ID3D11Texture2D> back_buffer() const;
        uint32_t current_back_buffer_index() const override
        {
            return 0;
        }

    private:
        com_unique_ptr<IDXGISwapChain>          m_swap_chain;
        com_unique_ptr<ID3D11Texture2D>         m_depth_stencil_buffer;
        com_unique_ptr<ID3D11DepthStencilState> m_depth_stencil_state;
    };

} // namespace mge::dx11