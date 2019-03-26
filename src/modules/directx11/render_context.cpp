#include "render_context.hpp"
#include "window.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(DX11);

namespace dx11 {

    render_context::render_context(window *w,
                                   const system_config &config)
        : mge::render_context(w)
        ,m_window(w)
    {
        init_context(config);
    }

    render_context::~render_context()
    {}

    void
    render_context::init_context(const system_config& config)
    {
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
        UINT flags = 0;
#ifdef MGE_DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        if(config.debug()) {
            flags |= D3D11_CREATE_DEVICE_DEBUG;
        }
        D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;
        if(config.software_device()) {
            driver_type = D3D_DRIVER_TYPE_WARP;
        }
        swap_chain_desc.BufferCount = 1;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferDesc.Width = m_window->rect().width();
        swap_chain_desc.BufferDesc.Height = m_window->rect().height();
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.SampleDesc.Count = 4; // TODO: multisampling configurable
        swap_chain_desc.Windowed = TRUE; // TODO: fullscreen
        swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.OutputWindow = m_window->hwnd();
        MGE_DEBUG_LOG(DX11) << "Create device and swap chain";

    }

}
